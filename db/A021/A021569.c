/*
 * A021569  Decimal expansion of 1/565.
 * https://oeis.org/A021569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/565) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th decimal digit of 1/565 via long division. */
seq_t a(idx_t n)
{
    idx_t rem = 1;       /* remainder of 1 divided by 565 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t tmp = rem * 10;
        digit = tmp / 565;
        rem   = tmp % 565;
    }
    return digit;
}
