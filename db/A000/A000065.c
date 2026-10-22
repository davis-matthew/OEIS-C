/*
 * A000065  -1 + number of partitions of n.
 * https://oeis.org/A000065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - 1, where p(n) is the number of partitions of n,
 *            computed with Euler's pentagonal number recurrence p(m) =
 *            Sum_{k>=1} (-1)^(k+1) * (p(m - k*(3k-1)/2) + p(m - k*(3k+1)/2)),
 *            p(0) = 1
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *memo_p = 0;
static size_t memo_cap = 0;  /* allocated slots */
static size_t memo_len = 0;  /* p(0..memo_len-1) are known */

static void ensure_p(size_t m)
{
    if (m < memo_len) return;
    if (m >= memo_cap) {
        size_t cap = memo_cap ? memo_cap : 64;
        while (cap <= m) cap *= 2;
        seq_t *np = alloc_series(cap);
        for (size_t i = 0; i < memo_len; i++) np[i] = memo_p[i];
        memo_p = np;  /* earlier buffer is deliberately leaked; growth doubles each time */
        memo_cap = cap;
    }
    if (memo_len == 0) {
        memo_p[0] = 1;
        memo_len = 1;
    }
    for (size_t i = memo_len; i <= m; i++) {
        seq_t pos = 0, neg = 0;
        for (size_t k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            size_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += memo_p[i - g1];
                if (g2 <= i) pos += memo_p[i - g2];
            } else {
                neg += memo_p[i - g1];
                if (g2 <= i) neg += memo_p[i - g2];
            }
        }
        memo_p[i] = pos - neg;
    }
    memo_len = m + 1;
}

seq_t a(idx_t n)
{
    ensure_p((size_t)n);
    return memo_p[n] - 1;
}
