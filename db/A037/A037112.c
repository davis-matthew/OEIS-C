/*
 * A037112  Trajectory of 3 under map n->29n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3, a(n+1) = (a(n)%2?29*a(n)+1 : a(n)/2)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 3;
    for (idx_t i = 0; i < n; ++i)
    {
        if (v & 1)           /* odd */
            v = v * 29 + 1;  /* 29*n + 1 */
        else                  /* even */
            v = v / 2;        /* n / 2 */
    }
    return v;
}
