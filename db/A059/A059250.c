/*
 * A059250  Square array read by antidiagonals: T(k,n) = binomial(n-1, k) +
 *          Sum_{i=0..k} binomial(n, i), k >= 1, n >= 0.
 * https://oeis.org/A059250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k,n) where the pair (k,n) is obtained by reading the
 *            square array T(k,n) by antidiagonals, with elements in each
 *            diagonal ordered from k = d down to 1 (i.e. decreasing n).
 *            T(k,n) is defined by T(k,n) = binomial(n-1,k) + sum_{i=0..k}
 *            binomial(n,i), with the convention binomial(n,k)=0 for...
 * Range:     u8 n <= 74, u16 n <= 316, u32 n <= 1208, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {          /* sequence is defined from offset 1; return 0 for safety */
        return (seq_t)0;
    }

    /* Find the smallest d with d*(d+1)/2 >= n */
    idx_t d = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while ((d * (d + 1) / 2) < n) d++;       /* adjust upward if needed */
    while (d > 0 && ((d - 1) * d / 2) >= n) d--;   /* adjust downward if overshot */

    idx_t total_before = (d - 1) * d / 2;   /* number of terms before diagonal d */
    idx_t pos = n - total_before - 1;        /* position within diagonal, 0-indexed */
    idx_t k      = d - pos;                  /* k decreases from d to 1 along the diagonal */
    idx_t nidx   = pos;                      /* the row index n of T(k,n) */

    /* Evaluate T(k,nidx) */
    if (nidx == 0) {
        return (seq_t)1;                     /* T(k,0) = 1 for all k >= 1 */
    }

    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {         /* sum_{i=0..k} binomial(nidx,i) */
        sum += binomial(nidx, i);
    }
    seq_t extra = binomial(nidx - 1, k);      /* binomial(nidx-1,k), zero if k > nidx-1 */
    return sum + extra;
}
