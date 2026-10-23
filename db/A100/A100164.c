/*
 * A100164  Structured rhombic triacontahedral numbers (vertex structure 11).
 * https://oeis.org/A100164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/3) * n * (5*n - 2) * (5*n - 4)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t t = (seq_t)5 * N;
    return N * (t - 2) * (t - 4) / 3;
}
