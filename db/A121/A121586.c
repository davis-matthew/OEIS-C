/*
 * A121586  Number of columns in all deco polyominoes of height n. A deco
 *          polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column.
 * https://oeis.org/A121586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)!*(n-1), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
    {
        return (seq_t)1;
    }

    seq_t fact = (seq_t)1;   /* fact holds (i-1)! during iteration i */
    seq_t an   = (seq_t)1;   /* a(0) */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t term = fact * (seq_t)(i - (idx_t)1);   /* (i-1)!*(i-1) */
        an = (seq_t)i * an + term;                   /* n*a(n-1) + term  */
        fact *= (seq_t)i;                             /* update to i!         */
    }

    return an;
}
