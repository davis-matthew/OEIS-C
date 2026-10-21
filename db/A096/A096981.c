/*
 * A096981  Number of partitions of n into parts congruent to {0, 1, 3, 5} mod
 *          6.
 * https://oeis.org/A096981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to
 *            {0,1,3,5} mod 6
 * Range:     u8 n <= 25, u16 n <= 65, u32 n <= 192, u64 n <= 626,
 *            u128 n <= 2190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple DP coin‑change style algorithm with memoisation.
   The array dp[i] stores a(i).  On each call with a larger n the table is
   recomputed from scratch up to that n.  This yields O(n²) time and O(n)
   memory, easily fast enough for n up to about 10 000. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* first allocation (for n == 0 or first call) */
    if (dp == NULL) {
        dp = (seq_t*)malloc(sizeof(seq_t));
        dp[0] = (seq_t)1;   /* a(0) = 1 */
        max_n = 0;
    }

    /* enlarge and recompute table if n exceeds current size */
    if (n > max_n) {
        idx_t new_n = n;
        dp = (seq_t*)realloc(dp, (new_n + 1) * sizeof(seq_t));

        /* initialise the table up to new_n */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= new_n; ++i)
            dp[i] = (seq_t)0;

        /* dynamic programming: for each allowed part add its contribution */
        for (idx_t part = 1; part <= new_n; ++part) {
            idx_t r = part % 6;
            if (r == 2 || r == 4)  /* parts congruent to 2 or 4 mod 6 are forbidden */
                continue;
            for (idx_t j = part; j <= new_n; ++j)
                dp[j] += dp[j - part];
        }
        max_n = new_n;
    }

    return dp[n];
}
