/*
 * A169607  a(n) = 7*A000330(n).
 * https://oeis.org/A169607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * n*(n+1)*(2*n+1)/6
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t n1 = n_t + 1;
    seq_t n2 = n_t * 2 + 1;
    seq_t prod = n_t * n1;
    prod *= n2;
    prod /= 6;
    prod *= 7;
    return prod;
}
