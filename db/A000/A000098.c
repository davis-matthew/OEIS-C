/*
 * A000098  Number of partitions of n if there are two kinds of 1, two kinds
 *          of 2 and two kinds of 3.
 * https://oeis.org/A000098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 1/((1-x)(1-x^2)(1-x^3)*Product_{k>=1} (1-x^k)); i.e. a(n)
 *            = Sum_{j=0..n} A000041(n-j)*A001399(j), with A000041 computed by
 *            Euler's pentagonal recurrence and A001399(j) =
 *            floor((j^2+6j+12)/12)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 87, u64 n <= 318,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_series.h"

static seq_t *P = 0;      /* partition numbers p(0..have-1) */
static size_t have = 0;

static void ensure(size_t m)
{
    size_t cap, i;
    seq_t *Q;
    if (m <= have) return;
    cap = have * 2;
    if (cap < m) cap = m;
    if (cap < 64) cap = 64;
    Q = alloc_series(cap);
    for (i = 0; i < have; i++) Q[i] = P[i];
    if (have == 0) { Q[0] = 1; have = 1; }
    for (i = have; i < cap; i++) {
        seq_t pos = 0, neg = 0;
        size_t k;
        for (k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            size_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                pos += Q[i - g1];
                if (g2 <= i) pos += Q[i - g2];
            } else {
                neg += Q[i - g1];
                if (g2 <= i) neg += Q[i - g2];
            }
        }
        Q[i] = pos - neg;
    }
    P = Q;      /* old buffer intentionally not freed */
    have = cap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t j;
    seq_t s = 0;
    ensure(N + 1);
    for (j = 0; j <= N; j++) {
        uint64_t jj = (uint64_t)j;
        uint64_t c = (jj * jj + 6 * jj + 12) / 12;
        s += P[N - j] * (seq_t)c;
    }
    return s;
}
