/*
 * A233252  Number of n X 4 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 160*a(n-1) - 2304*a(n-2), a(1)=10, a(2)=1184
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset, though the caller
       should never ask for n < 1. */
    if (n < 1) return 0;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)1184;

    seq_t prev2 = (seq_t)10;      /* a(1) */
    seq_t prev1 = (seq_t)1184;    /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur = prev1 * (seq_t)160 - prev2 * (seq_t)2304;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
