/*
 * A097036  Initial values for iteration of A063919[x] function such that
 *          iteration ends in a 3-cycle.
 * https://oeis.org/A097036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th x >= 1 (increasing) whose orbit under iteration of
 *            A063919 (sum of proper unitary divisors) ends in a cycle of
 *            length 3; found by search with orbit cycle detection
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 985 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A097036_s(uint64_t n) {
    if (n == 1) return 1;
    uint64_t t = n, prod = 1, p = 2;
    while (p <= t / p) {
        if (t % p == 0) {
            uint64_t pw = 1;
            while (t % p == 0) { t /= p; pw *= p; }
            prod *= 1 + pw;
        }
        p += (p == 2 ? 1 : 2);
    }
    if (t > 1) prod *= 1 + t;
    return prod - n;
}

static bool A097036_ends3(uint64_t x) {
    const int MAXS = 200;
    uint64_t h[200];
    uint64_t cur = x;
    for (int i = 0; i < MAXS; i++) {
        for (int j = 0; j < i; j++) {
            if (h[j] == cur) return (i - j) == 3;
        }
        h[i] = cur;
        if (cur > 1000000000ULL) return false;
        cur = A097036_s(cur);
    }
    return false;
}

seq_t a(idx_t n) {
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 1;
    while (cnt < (size_t)n) {
        if (A097036_ends3(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next;
        }
        next++;
    }
    return (seq_t)terms[n - 1];
}
