/*
 * A021609  Decimal expansion of 1/605.
 * https://oeis.org/A021609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th decimal digit of 1/605
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
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        digit = (seq_t)(rem / 605);
        rem %= 605;
    }
    return digit;
}
