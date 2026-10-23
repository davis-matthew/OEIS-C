/*
 * A021740  Decimal expansion of 1/736.
 * https://oeis.org/A021740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 736) mod 10 (decimal digit of 1/736)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t denom = 736;
    seq_t rem   = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * 10;
        digit = rem / denom;
        rem   = rem % denom;
    }
    return digit;
}
