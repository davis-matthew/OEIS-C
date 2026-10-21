/*
 * A231103  Number of n X 3 0..3 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2), a(1)=0, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Iteratively apply the recurrence */
    seq_t a_prev2 = (seq_t)0;   /* a(1) */
    seq_t a_prev1 = (seq_t)2;   /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)4 * a_prev1 + (seq_t)5 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
