/*
 * A059193  Engel expansion of 1/e = 0.367879... .
 * https://oeis.org/A059193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(1)=3, for n>1 : a(n) = 2*(2*n+1)*(n-1)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;

    seq_t nn = (seq_t)n;
    return (seq_t)2 * ((seq_t)2 * nn + (seq_t)1) * (nn - (seq_t)1);
}
