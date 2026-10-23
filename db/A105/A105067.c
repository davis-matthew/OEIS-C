/*
 * A105067  a(n) = Sum_{j=0..11} n^j.
 * https://oeis.org/A105067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..11} n^k
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t term = 1;
    seq_t n_t = (seq_t)n;
    for (idx_t i = 0; i <= 11; ++i) {
        sum += term;
        term *= n_t;
    }
    return sum;
}
