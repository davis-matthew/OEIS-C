/*
 * A192879  Coefficient of x in the reduction by (x^2 -> x + 1) of the
 *          polynomial p(n,x) given in Comments.
 * https://oeis.org/A192879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=4; a(n)=2*a(n-1)+2*a(n-2)-a(n-3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A192879.  The recurrence holds for all
 * n≥3, and the initial values are provided for n=0,1,2.  The function
 * performs an ordinal loop from 3 up to n, using only the allowed
 * arithmetic operations on seq_t.  No large intermediate values are
 * produced because the recurrence guarantees 2*a(n-1)+2*a(n-2) < 4*a(n-1),
 * which stays within the numeric range of seq_t for all n that fit in
 * seq_t. */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Use a sliding window of three consecutive values. */
    seq_t prev2 = (seq_t)0;   /* a(n-3) */
    seq_t prev1 = (seq_t)1;   /* a(n-2) */
    seq_t curr  = (seq_t)4;   /* a(n-1) */
    seq_t next;

    /* Compute iteratively up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        next = 2 * curr + 2 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }
    return curr;
}
