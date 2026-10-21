/*
 * A084100  Expansion of (1+x-x^2-x^3)/(1+x^2).
 * https://oeis.org/A084100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=1, a(n)=2*(-1)^{floor(n/2)} for n≥2
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                    /* a(0)=a(1)=1 */
    /* floor(n/2) parity determines the sign of 2 */
    return ( ((n/2) & 1) ? (seq_t)-2 : (seq_t)2 );
}
