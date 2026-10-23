/*
 * A154617  Eleven times hexagonal numbers: a(n) = 11*n*(2*n-1).
 * https://oeis.org/A154617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*n*(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    return (seq_t)11 * n_s * (((seq_t)2 * n_s) - (seq_t)1);
}
