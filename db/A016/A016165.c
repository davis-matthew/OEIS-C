/*
 * A016165  Expansion of 1/((1-5*x)*(1-11*x)).
 * https://oeis.org/A016165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 5^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) */
    seq_t p5     = 1;          /* 5^0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        p5 *= (seq_t)5;        /* compute 5^i */
        result *= (seq_t)11;   /* multiply previous a by 11 */
        result += p5;          /* add 5^i */
    }
    return result;
}
