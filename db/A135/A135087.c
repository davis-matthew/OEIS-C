/*
 * A135087  Triangle T(n, k) = 2*A134058(n, k) - 1, read by rows.
 * https://oeis.org/A135087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 158, u32 n <= 575, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A135087 (triangle read by rows).
   Formula: T(n,k) = 4*binomial(n,k) - (n==0?3:1).               */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
       The number of terms up to row r (inclusive) is
       (r+1)*(r+2)/2.                                        */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2) / 2) <= n ) r++;

    /* Row 0 is special: 1 */
    if (r == 0) return (seq_t)1;

    /* Number of terms before row r */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;                 /* column index within the row */

    /* Compute the entry */
    seq_t b = binomial(r, k);           /* binomial(r,k) as seq_t   */
    seq_t res = b * (seq_t)4;           /* 4*binomial(r,k)          */
    res -= (seq_t)1;                    /* subtract 1 for r>0 case   */
    return res;
}
