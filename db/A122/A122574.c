/*
 * A122574  a(1) = a(2) = 1, a(n) = -11*a(n-1) + a(n-2).
 * https://oeis.org/A122574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = a(2) = 1, a(n) = -11*a(n-1) + a(n-2) (for n ≥ 3)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 20, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined indices below the offset are undefined;
       returning 0 for n == 0 is safe but never used. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;

    /* Iterative computation of the recurrence.
       No dynamic memory or caching is required, keeping the
       running time O(n) and the required memory O(1). */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)1;   /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = - (seq_t)11 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
