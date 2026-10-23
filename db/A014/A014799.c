/*
 * A014799  Squares of odd pentagonal pyramidal numbers.
 * https://oeis.org/A014799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+2*n)^2*(1+4*n)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 12, u64 n <= 511, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (seq_t)1 + (seq_t)2 * nn;
    seq_t t2 = (seq_t)1 + (seq_t)4 * nn;
    seq_t t1sq = t1 * t1;
    seq_t t2sq = t2 * t2;
    seq_t t2four = t2sq * t2sq;
    return t1sq * t2four;
}
