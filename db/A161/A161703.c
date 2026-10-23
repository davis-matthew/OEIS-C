/*
 * A161703  a(n) = (4*n^3 - 12*n^2 + 14*n + 3)/3.
 * https://oeis.org/A161703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 12*n^2 + 14*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t term1 = 4 * n3;
    seq_t term2 = 12 * n2;
    seq_t term3 = 14 * N;

    /* compute (term1 + term3 + 3) - term2, avoiding underflow */
    seq_t sum = term1 + term3 + 3;
    sum -= term2;

    return sum / 3;
}
