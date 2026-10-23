/*
 * A027628  Expansion of Molien series for 5-dimensional group G_3 acting on
 *          Jacobi polynomials of ternary self-dual codes.
 * https://oeis.org/A027628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 + 13*n + 33*n^2 + 72*n^3 + 72*n^4) / 2 Valid for all n
 *            >= 0
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 103, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          // n^2
    seq_t n3 = n2 * N;         // n^3
    seq_t n4 = n3 * N;         // n^4

    seq_t sum = (seq_t)2
             + (seq_t)13 * N
             + (seq_t)33 * n2
             + (seq_t)72 * n3
             + (seq_t)72 * n4;

    return sum / (seq_t)2;
}
