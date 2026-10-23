/*
 * A124158  Maximal number of edges in a rectangle visibility graph with n
 *          nodes.
 * https://oeis.org/A124158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 for n <= 8, else 6*n-20
 * Range:     u8 n <= 45, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8) {
        seq_t m = (seq_t)n;
        return m * (m - (seq_t)1) / (seq_t)2;
    } else {
        seq_t m = (seq_t)n;
        return ((seq_t)6 * m) - (seq_t)20;
    }
}
