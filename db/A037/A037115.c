/*
 * A037115  Trajectory of 3 under map n->35n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; for n>0, a(n) = f(a(n-1)) where * f(x) = x/2 if x is
 *            even, * 35*x + 1 if x is odd
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 3;
    for (idx_t i = 0; i < n; ++i) {
        if ((cur & 1) == 0) {
            cur /= 2;
        } else {
            cur = cur * 35 + 1;
        }
    }
    return cur;
}
