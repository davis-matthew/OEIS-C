/*
 * A001401  Number of partitions of n into at most 5 parts.
 * https://oeis.org/A001401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + (a(n-2) + a(n-3) + a(n-4)) - (a(n-6) + 2*a(n-7) +
 *            a(n-8)) + (a(n-10) + a(n-11) + a(n-12)) - a(n-14), with a(k) = 0
 *            for k < 0 (gives a(0) = 1, a(1) = 1, a(2) = 2, ...)
 * Range:     u8 n <= 22, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a001401_dp = NULL;
static idx_t a001401_len = 0;

static seq_t a001401_get(idx_t i, idx_t k) {
    if (i < k) return (seq_t)0;
    return a001401_dp[i - k];
}

seq_t a(idx_t n) {
    if (n >= a001401_len) {
        idx_t newlen = n + 1;
        idx_t i;
        seq_t *nd = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
        for (i = 0; i < a001401_len; i++) nd[i] = a001401_dp[i];
        free(a001401_dp);
        a001401_dp = nd;
        for (i = a001401_len; i < newlen; i++) {
            seq_t pos = 1;
            seq_t neg = 0;
            pos += a001401_get(i, 2);
            pos += a001401_get(i, 3);
            pos += a001401_get(i, 4);
            neg += a001401_get(i, 6);
            neg += a001401_get(i, 7);
            neg += a001401_get(i, 7);
            neg += a001401_get(i, 8);
            pos += a001401_get(i, 10);
            pos += a001401_get(i, 11);
            pos += a001401_get(i, 12);
            neg += a001401_get(i, 14);
            a001401_dp[i] = pos - neg;
        }
        a001401_len = newlen;
    }
    return a001401_dp[n];
}
