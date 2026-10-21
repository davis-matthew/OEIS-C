/*
 * A212504  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and y>2z.
 * https://oeis.org/A212504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(3*n^2/4) * floor((n-1)^2/4)
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t tn = (seq_t)n;
    seq_t a_val = ((3 * tn * tn + 3) / 4);
    idx_t m_uint = n - 1;
    seq_t tm = (seq_t)m_uint;
    seq_t b_val = (tm * tm) / 4;
    return a_val * b_val;
}
