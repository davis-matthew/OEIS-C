/*
 * A007609  Values taken by the sigma function A000203, listed with
 *          multiplicity and in ascending order.
 * https://oeis.org/A007609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve sigma(m) for m <= M; since sigma(m) > m for m > 1, every m
 *            with sigma(m) <= M has m <= M. Take those values, sort them, and
 *            a(n) is the n-th smallest (with multiplicity). Double M until at
 *            least n values are <= M
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9078 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int a7609_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *a7609_cache = NULL;
static idx_t a7609_len = 0;

seq_t a(idx_t n) {
    if (n <= a7609_len) return (seq_t)a7609_cache[n - 1];
    uint64_t M = 2 * (uint64_t)n + 16;
    for (;;) {
        uint64_t *s = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
        for (uint64_t d = 1; d <= M; d++)
            for (uint64_t m = d; m <= M; m += d)
                s[m] += d;
        uint64_t cnt = 0;
        for (uint64_t m = 1; m <= M; m++)
            if (s[m] <= M) s[cnt++] = s[m];
        if (cnt >= n) {
            qsort(s, cnt, sizeof(uint64_t), a7609_cmp);
            free(a7609_cache);
            a7609_cache = s;
            a7609_len = cnt;
            return (seq_t)a7609_cache[n - 1];
        }
        free(s);
        M *= 2;
    }
}
