/*
 * A057094  Coefficient triangle for certain polynomials (rising powers).
 * https://oeis.org/A057094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the flattening (row‑major) of the triangle a(r,m)
 *            defined by a(0,0) = 0, a(r,m) = 0 if m < floor(r/2)+1, a(r,m) =
 *            ((-1)^(r-m+1))*binomial(m-1, r-m) otherwise (with 1≤r, m ≤ r)
 * Range:     i8 n <= 86, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle entries are summed in row‑major order:
   index n corresponds to (r,m) where r is the greatest integer
   satisfying r*(r+1)/2 ≤ n, and m = n - r*(r+1)/2.  */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       For the small n in the tests (≤ 10000) a simple loop is
       more robust than any floating point sqrt routine. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) {
        r++;
    }
    r--;                           /* now r is the correct row */

    idx_t m = n - (r * (r + 1)) / 2;   /* column within the row */

    /* Base case: a(0,0)=0 */
    if (r == 0)
        return 0;

    /* Entries outside the non‑zero band are zero.  */
    if (m < (r / 2 + 1))
        return 0;

    /* Sign factor (-1)^(r-m+1) */
    seq_t sign = ((r - m + 1) & 1) ? -1 : 1;

    /* Binomial part: C(m-1, r-m) */
    seq_t bin = binom(m - 1, r - m);

    return sign * bin;
}
