/*
 * A037105  Trajectory of 3 under map n->15n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3; for n>0, a(n) = (a(n-1) is odd) ? 15*a(n-1)+1 :
 *            a(n-1)/2;
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 3;
    idx_t i;
    for (i = 0; i < n; ++i)
    {
        if (x & 1)
            x = 15 * x + 1;
        else
            x = x / 2;
    }
    return x;
}
