/*
 * A233077  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 21*a(n-2), with a(1)=5, a(2)=95. The
 *            recurrence has characteristic roots 1 and 21, giving the closed
 *            form a(n) = (3*21^n + 7)/14, which yields integer values for all
 *            n≥1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n≥1.  For n=0 return 0, but this
       case is irrelevant for the OEIS offset. */
    if (n == 0)
        return 0;

    /* Handle the first two terms directly. */
    if (n == 1)
        return (seq_t)5;
    if (n == 2)
        return (seq_t)95;

    seq_t prev2 = (seq_t)5;   /* a(1) */
    seq_t prev1 = (seq_t)95;  /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = 22*a(i-1) - 21*a(i-2) */
        curr = ((seq_t)22 * prev1) - ((seq_t)21 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
