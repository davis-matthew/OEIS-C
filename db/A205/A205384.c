/*
 * A205384  Ordered differences of numbers s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(j) - s(i) where s(k) = C(2k,k)/2 and (i,j) correspond
 *            to n in the natural triangular enumeration: 1 ≤ i < j and n ≤
 *            (j-1)j/2
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row j such that n lies in the j‑th block
       of size j-1.  The total number of terms up to row j
       is T(j) = (j-1)j/2. */
    idx_t j = 2;
    idx_t tri = 1;                 /* T(2) */
    while (tri < n) {
        ++j;
        tri += j - 1;              /* add size of row j */
    }

    /* Determine the column index i inside the chosen row. */
    idx_t prev = tri - (j - 1);    /* T(j-1) */
    idx_t i = n - prev;            /* 1 ≤ i < j */

    /* Compute s(i) and s(j). */
    seq_t si = binom(2 * i, i);    /* C(2i,i)  */
    si = si / 2;                   /* divide by 2 */
    seq_t sj = binom(2 * j, j);    /* C(2j,j)  */
    sj = sj / 2;                   /* divide by 2 */

    return sj - si;                /* a(n) = s(j)-s(i) */
}
