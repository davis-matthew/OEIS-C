/*
 * A157447  a(n) = 512*n - 16.
 * https://oeis.org/A157447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 512*n - 16
 * Range:     u8 none, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * 512;
    res -= 16;
    return res;
}
