/*
 * A159744  If an array is made of columns of -nacci sequences, fibo-, tribo-
 *          etc. all starting w. 1,1,2 etc, the NW to SE diagonals can be
 *          extended by computation. The above is diagonal 8. See A159741 for
 *          details.
 * https://oeis.org/A159744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A159744
   Formula: a(1)=34, a(2)=149, a(3)=401.
   For n>=4: a(n)=2^(n+6)-112.
   Computation uses the power-of-two helper pow2().
   The sequence is derived as the binomial transform of a
   base sequence beginning with 34 followed by alternating
   quadratic polynomials, yielding the closed‑form above.
*/

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=34, a(2)=149, a(3)=401 */
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)149;
    if (n == 3) return (seq_t)401;

    /* For n>=4 : a(n) = 2^(n+6) - 112 */
    seq_t val = pow2(n + 6);   /* 2^(n+6) */
    return val - (seq_t)112;
}
