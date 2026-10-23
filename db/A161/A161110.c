/*
 * A161110  Number of partitions of n into nonzero triangular numbers where
 *          every part appears at least 9 times.
 * https://oeis.org/A161110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nonzero triangular numbers
 *            where every distinct part appears at least 9 times
 * Range:     u8 n <= 158, u16 n <= 458, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n) {
    /* handle trivial case quickly */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* allocate DP arrays */
    size_t size = (size_t)n + (size_t)1;
    seq_t *dp     = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *newdp  = (seq_t*)calloc(size, sizeof(seq_t));
    if (!dp || !newdp) {   /* allocation failed – return 0 */
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;   /* empty partition */

    /* iterate over triangular numbers t = k*(k+1)/2 */
    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;

        /* compute newdp from dp using the residue‑class method */
        for (idx_t r = 0; r < t; ++r) {
            seq_t sum = (seq_t)0;
            idx_t max_q = (n - r) / t;
            for (idx_t q = 0; q <= max_q; ++q) {
                idx_t s = r + q * t;
                if (q >= 9) {
                    sum += dp[(size_t)(r + (q - 9) * t)];
                }
                newdp[(size_t)s] = dp[(size_t)s] + sum;
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(newdp);
    return result;
}
