/*
 * A021257  Decimal expansion of 1/253.
 * https://oeis.org/A021257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/253) % 10 * Computes the (n+1)-th decimal
 *            digit of the fraction 1/253. * a(0) is the first digit after the
 *            decimal point
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t denom = (seq_t)253;        /* denominator */
    seq_t r = (seq_t)1;                    /* current remainder */
    seq_t digit = 0;                       /* digit at position n */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t r10 = r * (seq_t)10;         /* multiply remainder by 10 */
        digit = r10 / denom;               /* quotient is next digit */
        r = r10 % denom;                   /* new remainder */
    }

    return digit;
}
