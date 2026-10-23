/*
 * A121635  Number of deco polyominoes of height n, having no 2-cell columns
 *          starting at level 0. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(n) = (n-2)*[(n-2)! + a(n-1)] for n>=3.
 *            (Equivalently a(n) = (n-2)!(n^2-3n+4)/2 for n>=2.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;        /* a(1) = 1 */
    if (n == 2) return 1;        /* a(2) = 1 */

    seq_t fact = 1;              /* factorial of (k-2), starts with (3-2)!=1 */
    seq_t a_prev = 1;            /* a(2) */

    for (idx_t k = 3; k <= n; ++k)
    {
        fact = fact * (seq_t)(k - 2);          /* fact = fact * (k-2) = (k-2)! */
        seq_t temp = fact + a_prev;           /* (k-2)! + a_{k-1} */
        seq_t a_k   = (seq_t)(k - 2) * temp;  /* a_k = (k-2) * temp */
        a_prev = a_k;
    }
    return a_prev;               /* a(n) */
}
