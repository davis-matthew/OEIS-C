/*
 * A155194  Triangle read by rows: t(n,m)=If[m*(n - m) == 0, 1, n!*(1 -
 *          1/Binomial[n, m])].
 * https://oeis.org/A155194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factorial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A155194.
   The flattened triangle is read by rows.  For a given linear index n,
   we find the row r and column m such that offset_row(r) <= n < offset_row(r+1).
   The entry is
        t(r,m) = 1                if m==0 || m==r
        t(r,m) = r! - m!*(r-m)!   otherwise           (derived from
                              r!*(1-1/choose(r,m)) ).
   The function uses the helper routines factorial() and floor_sqrt()
   supplied in <../utils/factorial.h> and <../utils/floor_sqrt.h>.
   The calculation is O(1) apart from the factorial evaluation.
*/

#include "../utils/factorial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* find the row r: r(r+1)/2 <= n < (r+1)(r+2)/2 */
    seq_t temp = 8 * (seq_t)n + 1;          /* 8*n+1 for quadratic formula */
    seq_t sqrt_temp = floor_sqrt(temp);     /* integer sqrt */
    idx_t r = (idx_t)((sqrt_temp - 1) / 2); /* floor of (sqrt(8n+1)-1)/2 */

    /* row length is r+1, offset of row start is r*(r+1)/2 */
    idx_t offset = r * (r + 1) / 2;
    idx_t m = n - offset; /* column index within the row */

    /* end points contribute the value 1 */
    if (m == 0 || m == r)
        return (seq_t)1;

    /* compute factorials */
    seq_t fact_r  = factorial(r);
    seq_t fact_m  = factorial(m);
    seq_t fact_rm = factorial(r - m);

    /* t(r,m) = r! - m!*(r-m)! */
    seq_t sub = fact_m * fact_rm;
    return fact_r - sub;
}
