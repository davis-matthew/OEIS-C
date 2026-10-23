/*
 * A190340  Number of (n+1)X(n+1) symmetric binary matrices without the
 *          pattern 1 1 antidiagonally.
 * https://oeis.org/A190340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A190340: Number of (n+1)×(n+1) symmetric binary matrices
   without the pattern 1 1 antidiagonally.
   We use a recurrence for the factors r(n) of a(n):
       a(1) = 4
       a(n) = a(n-1) * r(n)   (n ≥ 2)

   The factor r(n) satisfies:
       r(2) = 3,   r(3) = 6
       For n ≥ 4:
           r(n) = r(n-1) + r(n-2) + s(n)
           s(n) =  1  if n%4 == 0 or n%4 == 3
                 = -1  otherwise
   This recurrence reproduces the known values:
   4, 12, 72, 720, 10800, 259200, 10368000, 673920000, 70087680000,
   11774730240000, … .
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; value for n=0 is undefined but we return 0. */
        return 0;
    }
    if (n == 1) return (seq_t)4;

    seq_t result = (seq_t)4;       /* a(1) */

    /* r(2) and r(3) are constants */
    seq_t r2 = (seq_t)3;
    result *= r2;                  /* a(2) = 4*3 = 12 */
    if (n == 2) return result;

    seq_t r3 = (seq_t)6;
    result *= r3;                  /* a(3) = 12*6 = 72 */
    if (n == 3) return result;

    /* For n ≥ 4 we use the recurrence for r(n) */
    seq_t r_prev2 = r2;            /* r(n-2) */
    seq_t r_prev1 = r3;            /* r(n-1) */

    for (idx_t k = 4; k <= n; ++k) {
        int sign = ( (k % 4 == 0) || (k % 4 == 3) ) ? 1 : -1;
        seq_t r_k = (sign == 1) ? (r_prev1 + r_prev2 + 1)
                                 : (r_prev1 + r_prev2 - 1);
        result *= r_k;
        r_prev2 = r_prev1;
        r_prev1 = r_k;
    }
    return result;
}
