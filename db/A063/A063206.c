/*
 * A063206  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          31 ).
 * https://oeis.org/A063206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -5/2 - (-1)^n/2 + 5*n for n>1, a(1)=2
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    seq_t val = (seq_t)n * (seq_t)5;
    return (n & 1) ? (val - (seq_t)2) : (val - (seq_t)3);
}
