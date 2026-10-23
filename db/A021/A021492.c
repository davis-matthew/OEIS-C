/*
 * A021492  Decimal expansion of 1/488.
 * https://oeis.org/A021492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in 1/488
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)(r * 10 / 488);
        r = r * 10 % 488;
    }
    return digit;
}
