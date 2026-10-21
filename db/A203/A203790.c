/*
 * A203790  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) + 6*a(n-2) , a(1)=784 , a(2)=10576 (Offset 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called below the offset, behaviour is undefined. Return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Base cases */
    if (n == 1)
        return (seq_t)784;
    if (n == 2)
        return (seq_t)10576;

    /* Compute the recurrence iteratively. */
    seq_t prev2 = (seq_t)784;   /* a(1)    */
    seq_t prev1 = (seq_t)10576; /* a(2)    */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)13 * prev1 + (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
