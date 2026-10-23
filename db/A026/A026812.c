/*
 * A026812  Number of partitions of n in which the greatest part is 6.
 * https://oeis.org/A026812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 6; otherwise a(n) = p_6(n-6), where p_6(m) is
 *            the number of partitions of m with all parts ≤ 6 (equivalently
 *            the coefficient of x^m in
 *            1/((1-x)(1-x^2)(1-x^3)(1-x^4)(1-x^5)(1-x^6)). This function
 *            computes the value in O(6*m) time and uses only integer
 *            arithmetic
 * Range:     u8 n <= 25, u16 n <= 84, u32 n <= 815, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;
    idx_t m = n - 6;                    /* shift by one mandatory part 6 */

    /* static DP table and its current size */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* enlarge and recompute the DP table if needed */
    if (dp_size <= m) {
        idx_t new_size = m + 1;          /* need indices 0..m inclusive */

        /* allocate new table */
        seq_t *newdp = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!newdp) return (seq_t)0;    /* malloc failure: return 0 */

        /* initialise */
        newdp[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i) newdp[i] = (seq_t)0;

        /* DP: for each part size 1..6, update counts */
        for (idx_t part = 1; part <= 6; ++part) {
            for (idx_t j = part; j < new_size; ++j) {
                newdp[j] += newdp[j - part];
            }
        }

        /* free old table, replace */
        free(dp);
        dp       = newdp;
        dp_size  = new_size;
    }

    /* dp[m] now holds the required value */
    return dp[m];
}
