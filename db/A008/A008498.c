/*
 * A008498  4-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^4 + 10*n^3 + 55*n^2 + 50*n + 24)/24
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 377, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = 5 * N * N * N * N
               + 10 * N * N * N
               + 55 * N * N
               + 50 * N
               + (seq_t)24;
    term /= (seq_t)24;
    return term;
}
