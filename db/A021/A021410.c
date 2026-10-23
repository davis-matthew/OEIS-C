/*
 * A021410  Decimal expansion of 1/406.
 * https://oeis.org/A021410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (floor(10^(n+1) / 406)) % 10 – the nth decimal digit of
 *            1/406
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10ULL;
        digit = (seq_t)(rem / 406ULL);
        rem %= 406ULL;
    }
    return digit;
}
