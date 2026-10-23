/*
 * A100405  Number of partitions of n where every part appears more than two
 *          times.
 * https://oeis.org/A100405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Product_{k>0} (1+x^(3k)/(1-x^k)) – partitions where each part
 *            appears at least 3 times
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *newdp = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *cdp = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp || !newdp || !cdp) return (seq_t)0;   /* out of memory – return 0 as safe fallback */

    /* initialise dp[0] = 1, others 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* copy dp into newdp (case of not using part k) */
        for (idx_t i = 0; i <= n; ++i) newdp[i] = dp[i];

        /* zero cdp for this k */
        for (idx_t i = 0; i <= n; ++i) cdp[i] = (seq_t)0;

        /* compute contributions of part k (at least 3 copies) */
        for (idx_t s = 3 * k; s <= n; ++s) {
            seq_t val = dp[s - 3 * k];          /* use exactly 3 copies of k */
            if (s >= k) val = val + cdp[s - k]; /* add more copies if already started */
            cdp[s] = val;
            newdp[s] = newdp[s] + val;          /* add to total ways for sum s */
        }

        /* prepare for next iteration: swap dp and newdp */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t result = dp[n];

    /* free memory */
    free(dp);
    free(newdp);
    free(cdp);

    return result;
}
