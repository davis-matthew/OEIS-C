/*
 * A233124  Number of n X 3 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or vertically, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) - 448*a(n-2) + 1024*a(n-3) for n ≥ 4 initial
 *            values: a(1) = 3, a(2) = 80, a(3) = 2688
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence is defined only for n ≥ 1 */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)80;
    if (n == 3)
        return (seq_t)2688;

    /* Keep only the last three values of the recurrence. */
    seq_t prev3 = (seq_t)3;       /* a(n-3)  */
    seq_t prev2 = (seq_t)80;      /* a(n-2)  */
    seq_t prev1 = (seq_t)2688;    /* a(n-1)  */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute 48*prev1 + 1024*prev3 first, then subtract 448*prev2.
           The intermediate sum is guaranteed to be ≥ the subtrahend,
           so the subtraction yields the correct signed result in
           unsigned arithmetic. */
        curr = (seq_t)48 * prev1 + (seq_t)1024 * prev3
             - (seq_t)448 * prev2;

        /* Shift the sliding window. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
