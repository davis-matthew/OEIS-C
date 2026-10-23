/*
 * A181390  Absolute difference between (sum of previous terms) and (n-th-odd
 *          square) with a(1) = 1.
 * https://oeis.org/A181390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=0; a(n)=8*(n-2) for n>2
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    return (seq_t)8 * ((seq_t)n - (seq_t)2);
}
