/*
 * A161052  Number of partitions of n into powers of two where every part
 *          appears at least 3 times.
 * https://oeis.org/A161052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161052:
   Number of partitions of n into powers of two where every part appears
   at least 3 times.  This is computed by dynamic programming over all
   powers of two <= n.  For each power p the recurrence is

        new_dp[i] = dp[i]                      // use p 0 times
                   + Σ_{k=3}^{⌊i/p⌋} dp[i - k*p]  // use p at least 3 times

   where dp is the array of counts before processing p.
   The implementation keeps a static cache so that repeated calls are
   fast and independent of call order. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cache[0..cached_n] */
static idx_t cached_n = 0;    /* largest n for which cache is valid */

/* Helper to free existing cache */
static void free_cache(void)
{
    if (cache != NULL)
    {
        free(cache);
        cache = NULL;
        cached_n = 0;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is undefined for n=0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* If we already have a value for this n, return it */
    if (n <= cached_n)
        return cache[n];

    /* ------------- Compute DP up to n ------------- */
    const size_t N = (size_t)n;         /* array bounds use size_t */
    seq_t *dp    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (dp == NULL || newdp == NULL)
    {
        free(dp); free(newdp);
        /* Fallback: free old cache and return 0 */
        free_cache();
        return (seq_t)0;
    }

    /* dp[0] = 1, others 0 */
    for (size_t i = 0; i <= N; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each power of two p <= n */
    for (idx_t p = 1; p <= n; p <<= 1)
    {
        /* Copy current dp (case of using the coin 0 times) */
        for (size_t i = 0; i <= N; ++i) newdp[i] = dp[i];

        /* For each residue class modulo p, add contributions of at least 3 copies */
        for (idx_t r = 0; r < p && r <= n; ++r)
        {
            seq_t sum = (seq_t)0;
            for (idx_t idx = r, m = 0; idx <= n; idx += p, ++m)
            {
                if (idx >= 3 * p)                /* k >= 3 */
                    sum += dp[(size_t)(idx - 3 * p)];
                newdp[(size_t)idx] = dp[(size_t)idx] + sum;
            }
        }

        /* Prepare for next power */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* dp now holds the final counts for 0..n.  Make it the cache. */
    free_cache();                      /* discard old cache */
    cache = dp;                         /* dp will be freed on next recompute */
    cached_n = n;

    /* Free the temporary buffer (which is the old dp pointer) */
    free(newdp);

    return cache[n];
}
