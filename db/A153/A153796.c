/*
 * A153796  6 times octagonal numbers: a(n) = 6*n*(3*n-2).
 * https://oeis.org/A153796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n*(3*n-2)
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 6 * N * ((seq_t)3 * N - 2);
}
