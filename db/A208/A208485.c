/*
 * A208485  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A208485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 14*a(n-2), a(1)=152, a(2)=1448
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial terms directly */
    if (n == (idx_t)1) return (seq_t)152;
    if (n == (idx_t)2) return (seq_t)1448;

    /* Recurrence: iterate up to the requested index */
    seq_t prev2 = (seq_t)152;   /* a(n-2) */
    seq_t prev1 = (seq_t)1448;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)11 * prev1 - (seq_t)14 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
