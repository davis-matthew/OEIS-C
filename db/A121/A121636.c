/*
 * A121636  Number of 2-cell columns starting at level 0 in all of deco
 *          polyominoes of height n. A deco polyomino is a directed
 *          column-convex polyomino in which the height, measured along the
 *          diagonal, is attained only in the last column.
 * https://oeis.org/A121636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-2)! + (n-1)*a(n-1) for n >= 3 with a(1) = 0, a(2) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* a(n-1) will be stored in a_prev.  fact = (n-2)!        */
    seq_t a_prev = (seq_t)1;     /* a(2) */
    seq_t fact = (seq_t)1;       /* (3-2)! = 1! when i starts at 3 */

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = i * (i-2)!     */
        seq_t term1 = fact * (seq_t)i;
        /* term2 = (i-1) * a(i-1) */
        seq_t term2 = a_prev * (seq_t)(i - 1);
        /* new_a = term1 + term2  */
        seq_t new_a = term1 + term2;

        /* prepare for next iteration */
        a_prev = new_a;
        fact   = fact * (seq_t)(i - 1);   /* (i-1)! becomes (i-2)! for next step */
    }

    return a_prev;   /* a(n) */
}
