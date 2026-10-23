/*
 * A029071  Expansion of 1/((1-x)*(1-x^4)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 30*n^2 + 261*n + 1296 + 27*n*[(n mod 2)=0] -
 *            24*n*[(n mod 3)=2])/1296)
 * Range:     u8 n <= 0, u16 n <= 30, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* compute numerator of the rational expression */
    seq_t num = nn * nn * nn;               /* n^3 */
    num += (seq_t)30 * nn * nn;             /* +30*n^2 */
    num += (seq_t)261 * nn;                 /* +261*n */
    num += (seq_t)1296;                     /* +1296 */

    /* add 27*n if n is even (n mod 2 == 0) */
    if ((n & 1) == 0)
        num += (seq_t)27 * nn;

    /* subtract 24*n if n mod 3 == 2 */
    if ((n % 3) == 2)
        num -= (seq_t)24 * nn;

    /* integer division gives the floor of the whole expression */
    return num / (seq_t)1296;
}
