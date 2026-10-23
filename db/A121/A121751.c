/*
 * A121751  Number of deco polyominoes of height n in which all columns end at
 *          an even level. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor((n-1)/2)*a(n-1) -
 *            (floor((n-1)/2)*floor((n-2)/2)-1)*a(n-2), a(1)=0, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t k = (seq_t)((i - 1) / 2);          /* floor((i-1)/2) */
        seq_t m = (seq_t)((i - 2) / 2);          /* floor((i-2)/2) */
        seq_t term1 = (k << 1) * a2;             /* 2*k*a(i-1) */
        seq_t term2 = ((k * m) - (seq_t)1) * a1; /* (k*m-1)*a(i-2) */
        seq_t ai = term1 - term2;                /* a(i) */
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
