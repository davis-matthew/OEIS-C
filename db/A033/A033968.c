/*
 * A033968  Trajectory of 1 under map n->23n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, a(n) = a(n-1)/2 if a(n-1) is even,
 *            23*a(n-1)+1 if a(n-1) is odd
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;                     /* a(0) = 1 */
    for (idx_t i = 0; i < n; ++i) {
        if ((x % 2) == 0)            /* even case */
            x = x / 2;
        else                        /* odd case */
            x = x * 23 + 1;
    }
    return x;
}
