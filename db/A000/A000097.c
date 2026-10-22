/*
 * A000097  Number of partitions of n if there are two kinds of 1's and two
 *          kinds of 2's.
 * https://oeis.org/A000097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)*(1-x^2)*Product_{k>=1}(1-x^k)); p(n) by Euler's
 *            pentagonal recurrence, A000070(n) = Sum_{k=0..n} p(k), a(n) =
 *            A000070(n) + a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 95, u64 n <= 341,
 *            u128 n <= 1280, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_series.h"

static seq_t *memo_a = 0;
static size_t memo_cap = 0;

static void build(size_t m)
{
    seq_t *p = alloc_series(m + 1);
    seq_t *res = alloc_series(m + 1);
    size_t i, k;
    seq_t cum;
    p[0] = 1;
    for (i = 1; i <= m; i++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            size_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                pos += p[i - g1];
                if (g2 <= i) pos += p[i - g2];
            } else {
                neg += p[i - g1];
                if (g2 <= i) neg += p[i - g2];
            }
        }
        p[i] = pos - neg;
    }
    cum = 0;
    for (i = 0; i <= m; i++) {
        cum += p[i];
        res[i] = cum;
        if (i >= 2) res[i] += res[i - 2];
    }
    memo_a = res;
    memo_cap = m + 1;
}

seq_t a(idx_t n)
{
    if ((size_t)n >= memo_cap) {
        size_t m = (size_t)n;
        if (m < 2 * memo_cap) m = 2 * memo_cap;
        if (m < 64) m = 64;
        build(m);
    }
    return memo_a[n];
}
