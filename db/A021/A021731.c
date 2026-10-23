/*
 * A021731  Decimal expansion of 1/727.
 * https://oeis.org/A021731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point of the decimal
 *            expansion of 1/727
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t P = (seq_t)727;   /* the divisor of the decimal expansion */

    seq_t rem = (seq_t)1;         /* start with numerator 1   */
    seq_t d;                      /* current digit */

    /* simulate long division up to the nth digit */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= (seq_t)10;
        d = rem / P;
        rem = rem % P;
    }

    return d;                     /* d holds the nth digit */
}
