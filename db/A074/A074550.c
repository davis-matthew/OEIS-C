/*
 * A074550  a(n) = 3^n + 4^n + 8^n.
 * https://oeis.org/A074550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 4^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    seq_t pow8 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;
        pow4 *= 4;
        pow8 *= 8;
    }
    return pow3 + pow4 + pow8;
}
