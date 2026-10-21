/*
 * A213041  Number of triples (w,x,y) with all terms in {0..n} and 2*|w-x| =
 *          max(w,x,y) - min(w,x,y).
 * https://oeis.org/A213041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 3 * Initial values:
 *            a(0) = 1, a(1) = 2, a(2) = 7, a(3) = 12. * This implementation
 *            evaluates the recurrence iteratively, avoiding * any dynamic
 *            allocation and running in O(n) time per call
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four base cases explicitly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)7;
    if (n == 3u) return (seq_t)12;

    /* Working variables for the recurrence. */
    seq_t prev4 = (seq_t)1;   /* a(0)   */
    seq_t prev3 = (seq_t)2;   /* a(1)   */
    seq_t prev2 = (seq_t)7;   /* a(2)   */
    seq_t prev1 = (seq_t)12;  /* a(3)   */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 2*a(i-3) + a(i-4) */
        curr = 2 * prev1 - 2 * prev3 + prev4;

        /* Shift the window for the next iteration. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
