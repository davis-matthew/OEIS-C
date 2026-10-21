/*
 * A086863  a(n)=floor{square((1*n^0+1*n^1+2*n^2+4*n^3)/(1*n^0+2*n^1+1*n^2))}.
 * https://oeis.org/A086863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor{ ((4*n^3+2*n^2+n+1)/(n+1)^2)^2 }
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute numerator and denominator of the fraction in 128‑bit to avoid overflow. */
    unsigned __int128 nn = n;                 /* cast to 128 for safe multiplications   */
    unsigned __int128 num128 = 4*nn*nn*nn      /* 4*n^3  */
                            + 2*nn*nn          /* 2*n^2  */
                            + nn               /* +n     */
                            + 1;               /* +1     */

    unsigned __int128 den128 = (nn + 1) * (nn + 1);   /* (n+1)^2   */

    /* Square the fraction by squaring numerator and denominator.  The division
       with 128‑bit integers stays exact because both numerator and denominator
       are even. */
    unsigned __int128 num2   = num128 * num128;
    unsigned __int128 den2   = den128 * den128;

    /* Floor of the rational: integer division in 128‑bit then cast back. */
    return (seq_t)(num2 / den2);
}
