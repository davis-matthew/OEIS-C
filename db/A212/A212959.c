/*
 * A212959  Number of (w,x,y) such that w,x,y are all in {0,...,n} and |w-x| =
 *          |x-y|.
 * https://oeis.org/A212959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 8*n + 3 + (-1)^n)/4
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 6 * nn * nn;      /* 6*n^2   */
    seq_t t2 = 8 * nn;          /* 8*n     */
    seq_t add = (n & 1) ? (seq_t)2 : (seq_t)4;  /* 3 + (-1)^n  -> 2 if odd, 4 if even */
    return (t1 + t2 + add) / 4;
}
