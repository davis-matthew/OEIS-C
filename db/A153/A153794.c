/*
 * A153794  4 times octagonal numbers: a(n) = 4*n*(3*n-2).
 * https://oeis.org/A153794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n*(3*n-2)
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    return 4 * n_s * (3 * n_s - 2);
}
