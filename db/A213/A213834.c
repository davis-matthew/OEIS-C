/*
 * A213834  Antidiagonal sums of the convolution array A213833.
 * https://oeis.org/A213834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2 - 2*n + 2)/6
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 250, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;
    seq_t term = (seq_t)3 * N * N - (seq_t)2 * N + 2;
    seq_t res = N * N1;
    res *= term;
    res /= 6;
    return res;
}
