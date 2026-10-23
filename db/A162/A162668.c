/*
 * A162668  a(n) = n*(n+1)*(n+2)*(n+3)/3.
 * https://oeis.org/A162668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)/3
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 334, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t prod = N;
    prod *= (N + 1);
    prod *= (N + 2);
    prod *= (N + 3);
    return prod / (seq_t)3;
}
