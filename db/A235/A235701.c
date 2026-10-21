/*
 * A235701  Number of n step walks (each step +- 1 starting from 0) which are
 *          never more than 6 or less than -6.
 * https://oeis.org/A235701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 66, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n‑step walks on the integer line starting at 0
   with steps ±1 that never go below –6 or above 6.  The function
   uses dynamic programming and memoises the results so each
   call is O(n) for the first time n is requested and O(1)
   thereafter. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Memoisation table for a(0)…a(current_max).  The table is
       grown automatically to the required size. */
    static seq_t *cache = NULL;
    static idx_t current_max = 0;          /* highest n we have computed */
    static seq_t dp[13]   = {0};           /* counts for current step */
    static seq_t next[13] = {0};           /* buffer for next step */

    /* One‑time initialisation. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* a(0) = 1 */
        cache[0] = (seq_t)1;
        dp[6] = (seq_t)1;                       /* position 0 is index 6 */
        current_max = 0;
    }

    /* Extend cache and DP up to n if necessary. */
    if (n > current_max) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

        for (idx_t step = current_max + 1; step <= n; ++step) {
            /* Compute transition for each position -6..6. */
            for (int i = 0; i < 13; ++i) {
                seq_t val = 0;
                if (i > 0)   val += dp[i - 1];   /* move from i-1 -> i */
                if (i < 12)  val += dp[i + 1];   /* move from i+1 -> i */
                next[i] = val;
            }

            /* Sum all positions to get a(step) */
            seq_t total = 0;
            for (int i = 0; i < 13; ++i)
                total += next[i];
            cache[step] = total;

            /* Prepare for next iteration */
            memcpy(dp, next, 13 * sizeof(seq_t));
        }
        current_max = n;
    }

    return cache[n];
}
