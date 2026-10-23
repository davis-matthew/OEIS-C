/*
 * A181475  a(n) = 3*n^4 + 6*n^3 - 3*n + 1.
 * https://oeis.org/A181475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4 + 6*n^3 - 3*n + 1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 194, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;

    seq_t result = (seq_t)3 * n4 + (seq_t)6 * n3 + (seq_t)1;
    result -= (seq_t)3 * N;
    return result;
}
