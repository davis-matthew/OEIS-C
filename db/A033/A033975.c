/*
 * A033975  Trajectory of 1 under map n->39n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, a(n) = 39*a(n-1)+1 if a(n-1) is odd, else
 *            a(n-1)/2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;                     /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        if (x & 1)                   /* odd */
            x = 39 * x + 1;
        else                         /* even */
            x >>= 1;                 /* equivalent to x / 2 */
    }
    return x;
}
