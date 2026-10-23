/*
 * A037102  Trajectory of 3 under map n->9n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; a(n)= (if a(n-1) odd) 9*a(n-1)+1 else a(n-1)/2, for n>0
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)3;          /* a(0) */
    if (n == 0)
        return x;
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((x & (seq_t)1) != 0)        /* odd */
            x = x * (seq_t)9 + (seq_t)1;
        else                            /* even */
            x = x / (seq_t)2;
    }
    return x;
}
