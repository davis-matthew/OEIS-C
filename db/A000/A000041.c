/*
 * A000041  a(n) is the number of partitions of n (the partition numbers).
 * https://oeis.org/A000041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, easy, nonn, nice, changed
 * Formula:   a(n) = Sum_{k>=1} (-1)^(k+1) * (a(n - k*(3k-1)/2) + a(n -
 *            k*(3k+1)/2)), a(0) = 1 (Euler's pentagonal number recurrence)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *memo = NULL;
static size_t memo_cap = 0;
static size_t memo_len = 0; /* number of computed terms */

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > memo_cap) {
        size_t cap = memo_cap ? memo_cap : 64;
        while (cap < need) cap *= 2;
        seq_t *m = alloc_series(cap);
        size_t i;
        for (i = 0; i < memo_len; i++) m[i] = memo[i];
        memo = m;
        memo_cap = cap;
    }
    while (memo_len < need) {
        size_t m = memo_len;
        seq_t pos = 0, neg = 0;
        if (m == 0) {
            pos = 1;
        } else {
            size_t k;
            for (k = 1;; k++) {
                size_t g1 = k * (3 * k - 1) / 2;
                size_t g2 = k * (3 * k + 1) / 2;
                if (g1 > m) break;
                if (k & 1) {
                    pos += memo[m - g1];
                    if (g2 <= m) pos += memo[m - g2];
                } else {
                    neg += memo[m - g1];
                    if (g2 <= m) neg += memo[m - g2];
                }
            }
        }
        memo[m] = pos - neg;
        memo_len++;
    }
    return memo[n];
}
