/*
 * A166450  a(n) = 3*a(n-2) for n > 2; a(1) = 1, a(2) = 6.
 * https://oeis.org/A166450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 6, a(n) = 3 * a(n-2) for n > 2
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)6;

    seq_t a_prev2 = (seq_t)1;   /* a(n-2) for the current iteration   */
    seq_t a_prev1 = (seq_t)6;   /* a(n-1) for the current iteration   */
    seq_t cur = 0;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        cur = a_prev2 * (seq_t)3;   /* a(i) = 3 * a(i-2)                */
        a_prev2 = a_prev1;          /* shift window                      */
        a_prev1 = cur;
    }
    return cur;
}
