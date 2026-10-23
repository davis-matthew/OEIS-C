/*
 * A033979  Trajectory of 1 under map n->47n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)= a(n-1)/2 if a(n-1) even, else 47*a(n-1)+1 (n>=1)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 18, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;                     /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        if ((x & 1) != 0)            /* current term is odd */
            x = x * 47 + 1;
        else
            x = x / 2;                /* x is even */
    }
    return x;
}
