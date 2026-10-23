/*
 * A021335  Decimal expansion of 1/331.
 * https://oeis.org/A021335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/331, with a(0) being the
 *            first digit after the decimal point
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Denominator 331 fits in any seq_t */
    const seq_t denom = 331;
    seq_t rem = 1;            /* remainder before multiplying by 10 */
    seq_t digit = 0;

    /* Perform the division algorithm n+1 times to reach the n-th digit */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                   /* shift decimal point left by one place */
        digit = rem / denom;         /* integer division gives the digit */
        rem %= denom;                /* remainder for next step */
    }
    return digit;
}
