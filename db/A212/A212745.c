/*
 * A212745  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          w=max{w,x,y,z}-2*min{w,x,y,z}.
 * https://oeis.org/A212745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of quadruples (w,x,y,z) with 0≤w,x,y,z≤n
 *            satisfying w = max{w,x,y,z} – 2·min{w,x,y,z}. The implementation
 *            counts all possible (m,M) with 0≤m≤M≤n and M≥3m, computes the
 *            number of ordered triples (x,y,z) that together with w meet the
 *            min/ max constraints, and sums the results
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t m = 0; m <= n; ++m) {
        /* smallest value in the quadruple */
        seq_t low = (seq_t)m;

        /* largest value must satisfy M ≥ 3m */
        idx_t M_start = 3 * m;
        if (M_start > n) continue;

        for (idx_t M = M_start; M <= n; ++M) {
            seq_t high   = (seq_t)M;
            seq_t len    = high - low + 1;        /* number of values in [low,high] */
            seq_t w      = high - 2 * low;        /* candidate value of w */

            seq_t total   = len * len * len;      /* all triples (x,y,z) */
            seq_t count;                          /* triples satisfying min/max */

            /* Special case: all four values equal */
            if (len == 1) {
                count = 1;                         /* only (m,m,m) for x,y,z */
            }
            /* w equals the minimum but not the maximum */
            else if (w == low) {
                seq_t without_high = (len - 1);
                without_high = without_high * without_high * without_high;
                count = total - without_high;
            }
            /* w equals the maximum but not the minimum */
            else if (w == high) {
                seq_t without_low = (len - 1);
                without_low = without_low * without_low * without_low;
                count = total - without_low;
            }
            /* w is neither the minimum nor the maximum */
            else {
                seq_t no_min  = (len - 1);
                no_min = no_min * no_min * no_min;
                seq_t no_max = no_min;                 /* same number of values */
                seq_t no_both = (len - 2);
                no_both = no_both * no_both * no_both;
                count = total - no_min - no_max + no_both;
            }

            sum += count;
        }
    }

    return sum;
}
