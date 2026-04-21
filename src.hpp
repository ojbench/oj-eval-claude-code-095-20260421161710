#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <cstddef>

struct Permutation { // 置换
    int* mapping;
    size_t size;

    Permutation(size_t size) : mapping(nullptr), size(size) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = static_cast<int>(i);
    }

    Permutation(const int* src, size_t n) : mapping(nullptr), size(n) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = src[i];
    }

    Permutation(const Permutation& other) : mapping(nullptr), size(other.size) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = other.mapping[i];
    }

    Permutation& operator=(const Permutation& other) {
        if (this == &other) return *this;
        int* new_map = new int[other.size];
        for (size_t i = 0; i < other.size; ++i) new_map[i] = other.mapping[i];
        delete[] mapping;
        mapping = new_map;
        size = other.size;
        return *this;
    }

    ~Permutation() {
        delete[] mapping;
        mapping = nullptr;
        size = 0;
    }

    void apply(int* permutation) const {
        int* tmp = new int[size];
        for (size_t i = 0; i < size; ++i) tmp[i] = permutation[mapping[i]];
        for (size_t i = 0; i < size; ++i) permutation[i] = tmp[i];
        delete[] tmp;
    }

    Permutation operator*(const Permutation& other) const {
        // (this * other)(i) = this(other(i)) => r[i] = this.mapping[ other.mapping[i] ]
        Permutation result(size);
        for (size_t i = 0; i < size; ++i) result.mapping[i] = mapping[other.mapping[i]];
        return result;
    }

    Permutation inverse() const {
        Permutation inv(size);
        for (size_t i = 0; i < size; ++i) inv.mapping[mapping[i]] = static_cast<int>(i);
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os << "[";
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) os << " ";
        }
        os << "]";
        return os;
    }
};

struct Transposition { // 对换
    int a, b;

    Transposition() : a(0), b(0) {}

    Transposition(int a_, int b_) : a(a_), b(b_) {}

    void apply(int* permutation, size_t size) const {
        if (a == b) return;
        if (static_cast<size_t>(a) < size && static_cast<size_t>(b) < size) {
            int t = permutation[a];
            permutation[a] = permutation[b];
            permutation[b] = t;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation p(size);
        if (a != b && static_cast<size_t>(a) < size && static_cast<size_t>(b) < size) {
            p.mapping[a] = b;
            p.mapping[b] = a;
        }
        return p;
    }
};

struct Cycle { // 轮换
    int* elements;
    size_t size;

    Cycle(const int* elems, size_t k) : elements(nullptr), size(k) {
        elements = new int[size];
        for (size_t i = 0; i < size; ++i) elements[i] = elems[i];
    }

    ~Cycle() {
        delete[] elements;
        elements = nullptr;
        size = 0;
    }

    void apply(int* permutation, size_t n) const {
        int* tmp = new int[n];
        for (size_t i = 0; i < n; ++i) tmp[i] = permutation[i];
        for (size_t j = 0; j < size; ++j) {
            int from = elements[(j + 1) % size];
            int to = elements[j];
            if (static_cast<size_t>(to) < n && static_cast<size_t>(from) < n) {
                permutation[to] = tmp[from];
            }
        }
        delete[] tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os << "{";
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) os << " ";
        }
        os << "}";
        return os;
    }

    Permutation toPermutation(size_t n) const {
        Permutation p(n);
        for (size_t j = 0; j < size; ++j) {
            int from = elements[j];
            int to = elements[(j + 1) % size];
            if (static_cast<size_t>(from) < n && static_cast<size_t>(to) < n) {
                p.mapping[from] = to;
            }
        }
        return p;
    }
};

#endif
