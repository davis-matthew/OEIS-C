/*
 * A073935  Numbers n such that the n-th row of triangle in A073932 contains
 *          exactly the divisors of n.
 * https://oeis.org/A073935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that the row of A073932 for m (m,
 *            f(m), f(f(m)), ..., 1 with f(m) = m - m/spf(m), f(prime) = m-1)
 *            is exactly the set of divisors of m; found by incremental search
 * Range:     u8 n <= 15, u16 n <= 42, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t A073935_f(uint64_t v) {
    if (v <= 1) return v;
    for (uint64_t i = 2; i * i <= v; ++i) {
        if (v % i == 0) return v - v / i;
    }
    return v - 1;
}

static uint64_t A073935_ndiv(uint64_t m) {
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            res *= (e + 1);
        }
    }
    if (m > 1) res *= 2;
    return res;
}

static int A073935_ok(uint64_t m) {
    uint64_t v = m, len = 0;
    for (;;) {
        if (m % v != 0) return 0;
        ++len;
        if (v == 1) break;
        v = A073935_f(v);
    }
    return len == A073935_ndiv(m);
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next = 1;
    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        uint64_t m = next++;
        if (A073935_ok(m)) {
            if (len == cap) {
                size_t nc = cap ? cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                if (!t) exit(1);
                cache = t;
                cap = nc;
            }
            cache[len++] = m;
        }
    }
    return (seq_t)cache[n - 1];
}
