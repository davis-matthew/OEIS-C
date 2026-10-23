/*
 * A144640  Row sums from A144562.
 * https://oeis.org/A144640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n - 1)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t an  = (seq_t)n;
    seq_t term = 2*an*an + 5*an;
    term = term - (seq_t)1;
    seq_t res = an * term / 2;
    return res;
}
