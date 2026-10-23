/*
 * A116445  Array read by antidiagonals: the binomial transform of the
 *          sequence (1,2,..n,0,0,0..) in row n.
 * https://oeis.org/A116445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A116445: array read by antidiagonals: the binomial transform of (1,2,…,n,0,0,0..) in row n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the anti‑diagonal index d so that
       n <= (d+1)(d+2)/2 and n > d(d+1)/2.  The total number
       of terms up to diagonal d is (d+1)(d+2)/2. */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2)) / 2 < n ) d++;

    /* Number of terms before diagonal d. */
    idx_t prev = d * (d + 1) / 2;

    /* Position of n inside diagonal d (0‑based). */
    idx_t offset = n - prev - 1;

    /* Row number r (1‑based) and column index k (0‑based). */
    idx_t r = d + 1 - offset;   /* rows counted from bottom to top */
    idx_t k = d - r + 1;        /* satisfies r-1 + k = d */

    /* Compute the binomial‑transform value for this cell:
       c(r,k) = sum_{i=0}^{min(k,r-1)} binom(k,i)*(i+1)           */
    idx_t m = (k < r - 1) ? k : r - 1;
    seq_t sum = 0;
    for (idx_t i = 0; i <= m; ++i) {
        seq_t bin        = binom(k, i);
        sum += bin * (seq_t)(i + 1);
    }
    return sum;
}
