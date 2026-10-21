/*
 * A094388  Expansion of (1- 2*x - x^2)/((1-x)*(1-3*x)).
 * https://oeis.org/A094388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0 a(n) = 3^(n-1) + 1 for n > 0 (expansion of
 *            (1-2x-x^2)/((1-x)*(1-3x))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;        /* start with 1 for +1 at the end */
    seq_t base   = (seq_t)3;        /* base of the powers of 3 */
    idx_t  exp   = n - 1;           /* exponent for 3^(n-1) */

    /* binary exponentiation */
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;        /* multiply when the bit is 1 */
        base *= base;              /* square the base */
        exp >>= (idx_t)1;          /* shift exponent right */
    }

    return result + (seq_t)1;       /* add the +1 from the formula */
}
