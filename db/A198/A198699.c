/*
 * A198699  a(n) = 7*10^n - 1.
 * https://oeis.org/A198699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*10^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= (seq_t)10;
    }
    return pow10 * (seq_t)7 - (seq_t)1;
}
