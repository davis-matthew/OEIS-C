/*
 * A211798  R(k,n) = Sum_{y=1..n} Sum_{x=1..n} floor((x^k + y^k)^(1/k)),
 *          square array read by descending antidiagonals.
 * https://oeis.org/A211798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(k,n) where * R(k,n) = Sum_{x=1..n} Sum_{y=1..n}
 *            floor((x^k + y^k)^(1/k)) * The pair (k,n) is obtained from the
 *            square array read by descending * antidiagonals
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility header providing seq_pow and seq_* arithmetic */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

static seq_t compute_R(idx_t k, idx_t n)
{
    /* k == 1: explicit formula */
    if (k == (idx_t)1) {
        seq_t tn = (seq_t)n;
        return tn * tn * (tn + (seq_t)1);
    }

    /* Pre‑compute x^k for 1 ≤ x ≤ n. */
    seq_t *powX = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!powX) return (seq_t)0;
    for (idx_t x = (idx_t)1; x <= n; ++x)
        powX[x] = seq_pow((seq_t)x, k);

    /* Pre‑compute m^k for 0 ≤ m ≤ 2*n (upper bound for the kth root). */
    size_t mid_max = (size_t)(2 * n);
    seq_t *powMid = (seq_t *)malloc((mid_max + 1) * sizeof(seq_t));
    if (!powMid) {
        free(powX);
        return (seq_t)0;
    }
    for (idx_t m = (idx_t)0; m <= (idx_t)mid_max; ++m)
        powMid[m] = seq_pow((seq_t)m, k);

    seq_t total = (seq_t)0;

    /* Main double sum. */
    for (idx_t x = (idx_t)1; x <= n; ++x) {
        for (idx_t y = (idx_t)1; y <= n; ++y) {
            seq_t sum_xy = powX[x] + powX[y];

            /* Binary search for floor((x^k + y^k)^(1/k)). */
            idx_t low = (idx_t)0;
            idx_t high = x + y;
            idx_t best = (idx_t)0;
            while (low <= high) {
                idx_t mid = low + (high - low) / (idx_t)2;
                seq_t mid_pow = powMid[mid];
                if (mid_pow <= sum_xy) {
                    best = mid;
                    low = mid + (idx_t)1;
                } else {
                    if (mid == (idx_t)0) break; /* safety */
                    high = mid - (idx_t)1;
                }
            }
            total += (seq_t)best;
        }
    }

    free(powX);
    free(powMid);
    return total;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;           /* off‑offset */

    /* Find the diagonal (t) such that t(t+1)/2 ≥ n. */
    idx_t t = (idx_t)1;
    while (t * (t + (idx_t)1) / (idx_t)2 < n) ++t;

    /* Number of terms before this diagonal. */
    idx_t prev = (t - (idx_t)1) * t / (idx_t)2;
    /* Offset within the diagonal (1‑based). */
    idx_t offset = n - prev;

    /* Coordinates: k = offset, n_val = t - offset + 1. */
    idx_t k     = offset;
    idx_t n_val = t - offset + (idx_t)1;

    return compute_R(k, n_val);
}
