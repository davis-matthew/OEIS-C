/*
 * A135300  Positive X-values of solutions to the equation 1!*X^4 - 2!*(X +
 *          1)^3 + 3!*(X + 2)^2 - (4^2)*(X + 3) + 5^2 = Y^3.
 * https://oeis.org/A135300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=n^3-1 for n≥2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * nn;   /* compute n^3 */
    return res - (seq_t)1;
}
