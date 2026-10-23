/*
 * A020039  Nearest integer to Gamma(n + 2/5)/Gamma(2/5).
 * https://oeis.org/A020039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to Γ(n + 2/5)/Γ(2/5)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow for n up to the
       largest value that fits into seq_t.  For arbitrary‑precision
       seq_t the cast to seq_t at the end creates the big integer. */
    unsigned __int128 num = 1;           /* numerator ∏(5k+2)    */
    unsigned __int128 den = 1;           /* denominator 5ⁿ         */
    for (idx_t k = 0; k < n; ++k)
    {
        num *= (unsigned __int128)(5 * k + 2);  /* term (5k+2)      */
        den *= 5;                            /* multiply denominator by 5 */
    }

    /* Rounding: round(x) = floor((x + 1/2)) = (num + den/2)/den  */
    unsigned __int128 half      = den >> 1;            /* floor(den/2)    */
    unsigned __int128 rounded   = (num + half) / den;   /* floor((num+half)/den) */

    return (seq_t)rounded;   /* cast to user supplied seq_t type */
}
