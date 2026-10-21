/*
 * A083813  a(n) = 3*(10^n - 1).
 * https://oeis.org/A083813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= 10;
    }
    return pow10 * 3 - 3;
}
