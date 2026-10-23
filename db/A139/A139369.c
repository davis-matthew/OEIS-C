/*
 * A139369  Array read by antidiagonals, n-th sum of 2 distinct powers of k.
 * https://oeis.org/A139369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k^i + k^j where (n-1) is read by antidiagonals of the
 *            infinite array whose rows are k = 2,3,4,... and the columns
 *            enumerate the distinct pairs (i,j) with i < j in the order: j =
 *            1, 2, 2, 3, 3, 3, 4, 4, 4, 4, ... That is, for each j ≥ 1 the
 *            pairs (i,j) appear with i = 0,1,…,j-1. The overall...
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is 1‑based, value for n=0 not used */

    /* Convert to 0‑based index */
    idx_t m = n - 1;

    /* Find the diagonal d such that the number of entries up to
       but not including this diagonal is <= m < number up to and
       including this diagonal.  The number of entries in diagonal d
       is d+1, so the cumulative count up to diagonal d-1 is
       T(d) = d(d+1)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= m) d++;

    idx_t prev = d * (d + 1) / 2;   /* entries before diagonal d    */
    idx_t p    = m - prev;          /* position within diagonal d   */

    /* Row index starts at 0 for k=2, so k = row + 2.  The traversal
       order in each diagonal is from the largest row to the smallest,
       thus row = d - p.                                      */
    idx_t r = d - p;                 /* row index, 0 based          */
    idx_t c = p;                     /* column index, 0 based       */
    idx_t k = r + 2;                 /* base of the powers          */

    /* Map column index c to the pair (i,j).  For a fixed j,
       there are j entries corresponding to i = 0..j-1.
       The cumulative number of entries up to j is j(j+1)/2.
       Find the smallest j such that j(j+1)/2 > c.              */
    idx_t j = 1;
    while (j * (j + 1) / 2 <= c) j++;

    /* Now i = c - j(j-1)/2, the offset within the block for this j */
    idx_t i = c - j * (j - 1) / 2;

    /* Compute k^i and k^j using the provided big‑power routine */
    seq_t pow_i = seq_pow((seq_t)k, (idx_t)i);
    seq_t pow_j = seq_pow((seq_t)k, (idx_t)j);

    return pow_i + pow_j;
}
