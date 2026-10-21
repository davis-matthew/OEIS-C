/*
 * A057728  A triangular table of decreasing powers of two (with first column
 *          all ones).
 * https://oeis.org/A057728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) is 1 when n is the first or last element of a triangular *
 *            row; otherwise a(n) = 2^{r-p}, where r is the index of the row
 *            containing * n (rows start at 1), and p = n - r*(r-1)/2 is the
 *            position within that row
 * Range:     u8 n <= 46, u16 n <= 154, u32 n <= 562, u64 n <= 2146,
 *            u128 n <= 5050, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but return 0 */
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r + 1) <= n && (r * (r + 1) / 2) < n) r++;
    /* r is now the row index */
    idx_t prev_tri = r * (r - 1) / 2;   /* sum of rows before r */
    idx_t p = n - prev_tri;             /* position within row r */
    if (p == 1 || p == r)
        return (seq_t)1;                /* first or last entry of the row */
    idx_t exp = r - p;                  /* exponent of 2 for the entry */
    return pow2(exp);
}
