/*
 * A022278  a(n) = n*(21*n-1)/2.
 * https://oeis.org/A022278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(21*n-1)/2
 * Range:     u8 n <= 3, u16 n <= 78, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn == 0) return (seq_t)0;
    seq_t val = nn * ((seq_t)21 * nn - (seq_t)1);
    return val / 2;
}
