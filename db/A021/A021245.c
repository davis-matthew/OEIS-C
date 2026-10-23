/*
 * A021245  Decimal expansion of 1/241.
 * https://oeis.org/A021245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 241) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 241;
    idx_t rem = 1;          /* numerator remainder of 1/241 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        digit = rem / N;
        rem %= N;
    }
    return digit;
}
