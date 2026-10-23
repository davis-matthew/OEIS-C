/*
 * A198854  a(n) = 6*8^n-1.
 * https://oeis.org/A198854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*8^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i) {
        val *= (seq_t)8;
    }
    return val * (seq_t)6 - (seq_t)1;
}
