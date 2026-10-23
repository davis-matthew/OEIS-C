/*
 * A033980  Trajectory of 1 under map n->49n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th iterate of f starting at 1, where f(x) = 49*x +
 *            1 if x is odd, x/2 if x is even
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((x & 1) == 0)   /* even */
            x = x >> 1;
        else                /* odd */
            x = x * 49 + 1;
    }
    return x;
}
