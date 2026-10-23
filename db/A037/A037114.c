/*
 * A037114  Trajectory of 3 under map n->33n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; for n>0, a(n)=33*a(n-1)+1 if a(n-1) odd, otherwise
 *            a(n)=a(n-1)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 3;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((cur & 1) == 1)          /* odd */
            cur = 33 * cur + 1;
        else                         /* even */
            cur >>= 1;               /* divide by 2 */
    }
    return cur;
}
