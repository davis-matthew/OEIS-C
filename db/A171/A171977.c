/*
 * A171977  a(n) = 2^(k+1) where 2^k is the highest power of 2 dividing n.
 * https://oeis.org/A171977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^(k+1) where 2^k is the largest power of 2 dividing n.
 *            Computation: find the lowest set bit of n (n & -n) and double
 *            it. This works for any unsigned seq_t type and is O(1) per call
 * Range:     u8 n <= 127, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 79 terms through n = 9627 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;      /* interpret n as the same unsigned type */
    seq_t low = t & -(t);    /* lowest power of two dividing n, i.e. 2^k */
    return low << 1;         /* return 2^(k+1) */
}
