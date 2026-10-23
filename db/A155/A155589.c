/*
 * A155589  a(n) = 6^n + 2^n - 1.
 * https://oeis.org/A155589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 2^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow6 = 1;
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow6 *= 6;
        pow2 *= 2;
    }
    return pow6 + pow2 - (seq_t)1;
}
