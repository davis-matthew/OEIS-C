/*
 * A121633  Sum of the bottom levels of the last column over all deco
 *          polyominoes of height n. A deco polyomino is a directed
 *          column-convex polyomino in which the height, measured along the
 *          diagonal, is attained only in the last column.
 * https://oeis.org/A121633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)! - 1 (a(1) = 0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t a_val = (seq_t)0;   /* a(1) */
    seq_t fact  = (seq_t)1;   /* (n-1)! for n=2 starts at 1! */

    for (idx_t i = 2; i <= n; ++i) {
        a_val = (seq_t)i * a_val + fact - (seq_t)1;
        fact  *= (seq_t)i;    /* update to i! for next iteration */
    }
    return a_val;
}
