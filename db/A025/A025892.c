/*
 * A025892  Expansion of 1/((1-x^5)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A025892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((19*n^2+7*n)/18) - floor((3*n^2-2)/5) -
 *            floor((5*n^2+4*n+5)/11)
 * Range:     u8 n <= 490, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return 1;

    /* Use signed 64‑bit arithmetic to handle the possible negative numerator
       of the second floor term when n = 0.  For n >= 1 all numerators are
       non‑negative. */
    int64_t nn = (int64_t)n;

    int64_t term1 = (19*nn*nn + 7*nn) / 18;          /* floor division, positive */
    int64_t term2 = (3*nn*nn - 2) / 5;               /* positive for n>=1   */
    int64_t term3 = (5*nn*nn + 4*nn + 5) / 11;        /* positive            */

    int64_t res = term1 - term2 - term3;
    return (seq_t)res;
}
