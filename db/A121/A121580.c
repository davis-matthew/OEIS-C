/*
 * A121580  Number of cells in column 1 of all deco polyominoes of height n. A
 *          deco polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column.
 * https://oeis.org/A121580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=a(n-1)+(n-1)!*(1+n*(n-1)/2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, but safe */
    if (n == 1)
        return 1;                 /* a(1)=1 */

    seq_t fact = 1;               /* fact = (k-1)! in the loop */
    seq_t result = 1;             /* current a(k) */

    for (idx_t k = 2; k <= n; ++k) {
        /* term = k*(k-1)/2  (small integer, cast to seq_t first) */
        seq_t term = (seq_t)k * (seq_t)(k - 1) / 2;
        /* add = fact * (1 + term) */
        seq_t add = fact * (1 + term);
        result += add;           /* a(k) = a(k-1) + add */
        fact *= (seq_t)k;        /* update fact to k! for next iteration */
    }
    return result;
}
