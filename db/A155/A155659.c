/*
 * A155659  a(n) = 8^n - 7^n + 1.
 * https://oeis.org/A155659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 7^n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow8 = 1;
    seq_t pow7 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow8 *= (seq_t)8;
        pow7 *= (seq_t)7;
    }
    return pow8 - pow7 + 1;
}
