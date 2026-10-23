/*
 * A025939  Expansion of 1/((1-2*x)*(1-3*x)*(1-6*x)*(1-9*x)).
 * https://oeis.org/A025939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 135*a(n-2) + 360*a(n-3) - 324*a(n-4), with
 *            initial values a(0)=1, a(1)=20, a(2)=265, a(3)=2960
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)20;
    if (n == 2u) return (seq_t)265;
    if (n == 3u) return (seq_t)2960;

    /* Sliding window of the last four terms. */
    seq_t prev4 = (seq_t)1;          /* a(n-4) */
    seq_t prev3 = (seq_t)20;         /* a(n-3) */
    seq_t prev2 = (seq_t)265;        /* a(n-2) */
    seq_t prev1 = (seq_t)2960;       /* a(n-1) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 4u; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        cur = (seq_t)((seq_t)20 * prev1
                    - (seq_t)135 * prev2
                    + (seq_t)360 * prev3
                    - (seq_t)324 * prev4);

        /* Shift the window for the next iteration. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
