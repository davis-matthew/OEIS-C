/*
 * A016061  a(n) = n*(n+1)*(4*n+5)/6.
 * https://oeis.org/A016061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n+5)/6
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t term = tn * (tn + 1) * (4 * tn + 5) / 6;
    return term;
}
