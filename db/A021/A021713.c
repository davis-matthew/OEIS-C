/*
 * A021713  Decimal expansion of 1/709.
 * https://oeis.org/A021713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/709, with offset 0
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1/709 = 0.0014104372355430183356840620592383638928067700987306... */
seq_t a(idx_t n)
{
    idx_t rem = 1;   /* remainder before computing the digit */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        digit = (seq_t)(rem * 10) / 709;
        rem   = (rem * 10) % 709;
    }
    return digit;
}
