/*
 * A061804  a(n) = 2*n*(2*n^2 + 1).
 * https://oeis.org/A061804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(2*n^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t prod = 2 * n_s;
    prod *= (2 * n_s * n_s + 1);
    return prod;
}
