/*
 * A000094  Number of trees of diameter 4.
 * https://oeis.org/A000094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n-1) - (n-1) for n >= 2, a(1) = 0; A000041 is
 *            computed by Euler's pentagonal number recurrence p(m) =
 *            Sum_{k>=1} (-1)^(k+1) * (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0)
 *            = 1
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 128, u64 n <= 417,
 *            u128 n <= 500, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *P = NULL;
static size_t P_cap = 0;   /* allocated slots */
static size_t P_cnt = 0;   /* computed entries: P[0..P_cnt-1] */

static void ensure_partitions(size_t m)
{
    size_t i;
    if (m < P_cnt)
        return;
    if (m + 1 > P_cap) {
        size_t ncap = P_cap ? P_cap : 64;
        seq_t *np;
        while (ncap < m + 1)
            ncap *= 2;
        np = alloc_series(ncap);
        for (i = 0; i < P_cnt; i++)
            np[i] = P[i];
        P = np;            /* old buffer intentionally kept (not freed) */
        P_cap = ncap;
    }
    if (P_cnt == 0) {
        P[0] = 1;
        P_cnt = 1;
    }
    for (i = P_cnt; i <= m; i++) {
        seq_t pos = 0, neg = 0;
        size_t k;
        for (k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            size_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i)
                break;
            if (k & 1) {
                pos += P[i - g1];
                if (g2 <= i)
                    pos += P[i - g2];
            } else {
                neg += P[i - g1];
                if (g2 <= i)
                    neg += P[i - g2];
            }
        }
        P[i] = pos - neg;
    }
    P_cnt = m + 1;
}

seq_t a(idx_t n)
{
    size_t m;
    if (n < 2)
        return 0;
    m = (size_t)(n - 1);
    ensure_partitions(m);
    return P[m] - (seq_t)m;
}
