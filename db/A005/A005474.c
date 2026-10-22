/*
 * A005474  Class numbers of the real quadratic fields Q(sqrt(A005473(n))).
 * https://oeis.org/A005474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(p), the class number of Q(sqrt(p)), where p =
 *            A005473(n) is the n-th prime of the form k^2+4 (k odd). The
 *            fundamental unit is eps = (k+sqrt(p))/2, R = log(eps), and h is
 *            the rounded value of h*R = (1/2) Sum_{m>=1} (m/p) * [ sqrt(p)/m
 *            * erfc(m*sqrt(Pi/p)) + E1(Pi*m^2/p) ] (Cohen's rapidly...
 * Range:     u8 n <= 270, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   expint1, jacobi_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/expint1.h"
#include "../utils/jacobi_u.h"

static uint64_t mulmod_u(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t powmod_u(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod_u(r, b, m);
        b = mulmod_u(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, j;
    uint64_t d = n - 1;
    int s = 0;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod_u(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < s; j++) {
            x = mulmod_u(x, x, n);
            if (x == n - 1) break;
        }
        if (j == s) return false;
    }
    return true;
}

static uint64_t *ks = NULL;
static size_t ks_cnt = 0, ks_cap = 0;
static uint64_t ks_next = 1;

static uint64_t kth(idx_t n) {
    while (ks_cnt < (size_t)n) {
        uint64_t k = ks_next;
        ks_next += 2;
        if (mr_prime(k * k + 4)) {
            if (ks_cnt == ks_cap) {
                ks_cap = ks_cap ? 2 * ks_cap : 1024;
                ks = (uint64_t *)realloc(ks, ks_cap * sizeof(uint64_t));
            }
            ks[ks_cnt++] = k;
        }
    }
    return ks[n - 1];
}

seq_t a(idx_t n) {
    const double PI = 3.14159265358979323846;
    uint64_t k = kth(n);
    uint64_t p = k * k + 4;
    double D = (double)p;
    double sD = sqrt(D);
    double c = sqrt(PI / D);
    double R = log(((double)k + sD) / 2.0);
    uint64_t N = (uint64_t)sqrt(45.0 * D / PI) + 2;
    uint64_t m;
    double sum = 0.0, hval;
    long long h;
    for (m = 1; m <= N; m++) {
        int chi = jacobi_u(m, p);
        double md, t;
        if (chi == 0) continue;
        md = (double)m;
        t = sD / md * erfc(md * c) + expint1(PI * md * md / D);
        if (chi > 0) sum += t; else sum -= t;
    }
    hval = 0.5 * sum / R;
    h = (long long)(hval + 0.5);
    return (seq_t)(uint64_t)h;
}
