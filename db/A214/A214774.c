/*
 * A214774  Number of ways of obtaining a weight of n grams using eight
 *          weights of denominations 1, 1, 2, 5, 10, 10, 20 and 50 grams.
 * https://oeis.org/A214774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A214774:
   Number of ways to obtain a weight of n grams using the eight
   weights 1, 1, 2, 5, 10, 10, 20 and 50 (each weight may be used at
   most once).  It returns 0 for n > 99. */
#include <stdbool.h>
#include <stddef.h>

#define MAX_N 99

seq_t a(idx_t n)
{
    /* DP table for subset-sum counts – computed once */
    static seq_t dp[MAX_N + 1];
    static bool initialized = false;

    /* the eight distinct weights */
    static const idx_t weights[8] = {1, 1, 2, 5, 10, 10, 20, 50};

    if (!initialized) {
        /* initialise DP table */
        for (int i = 0; i <= MAX_N; ++i)
            dp[i] = 0;
        dp[0] = 1;  /* one way to make weight 0 – choose nothing */

        /* subset‑sum DP: each weight can be taken at most once */
        for (int wi = 0; wi < 8; ++wi) {
            int w = (int)weights[wi];
            for (int s = MAX_N; s >= w; --s) {
                dp[s] += dp[s - w];
            }
        }
        initialized = true;
    }

    if (n <= (idx_t)MAX_N)
        return dp[(size_t)n];
    return (seq_t)0;
}
