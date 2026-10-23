/*
 * A033972  Trajectory of 1 under map n->33n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = (a(n-1) % 2 == 0 ? a(n-1)/2 :
 *            33*a(n-1)+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;
    if (n == 0) return x;
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((x & 1) == 0)
            x >>= 1;          /* even case: divide by 2 */
        else
            x = x * 33 + 1;    /* odd case: 33*n + 1 */
    }
    return x;
}
