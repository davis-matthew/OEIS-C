/*
 * A025147  Number of partitions of n into distinct parts >= 2.
 * https://oeis.org/A025147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of partitions of n into distinct parts ≥ 2
 *            (generating function ∏_{k≥2}(1+x^k))
 * Range:     u8 n <= 33, u16 n <= 85, u32 n <= 249, u64 n <= 813,
 *            u128 n <= 2862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *dp      = NULL;   /* array dp[0..dp_size-1] */
    static size_t dp_size = 0;      /* current size of dp array */
    static idx_t  last_n  = 0;      /* largest n for which dp was computed */

    /* Resize or allocate dp array if needed */
    size_t new_size = (size_t)(n + 1);   /* number of entries needed */
    if (dp_size < new_size)
    {
#ifdef __cplusplus
        /* C++: use new/delete to construct objects correctly */
        seq_t *tmp = new seq_t[new_size];
        if (dp)
        {
            for (size_t i = 0; i < dp_size; ++i)
                tmp[i] = dp[i];
            delete[] dp;
        }
        dp = tmp;
#else
        /* C: use realloc which does not call constructors (seq_t is an
           ordinary integer type in the C build). */
        seq_t *tmp = realloc(dp,
                              new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure – value not defined */
        dp = tmp;
#endif
        dp_size = new_size;
    }

    /* Recompute from scratch if we have not processed up to n yet */
    if (new_size > last_n)
    {
        /* initialise for all indices up to n */
        for (size_t i = 0; i < new_size; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;               /* empty partition of 0 */

        /* dynamic programming over distinct parts ≥ 2 */
        for (idx_t k = 2; k <= n; ++k)
        {
            /* iterate i from n down to k to avoid reusing part k multiple times */
            for (size_t i = (size_t)n; i >= (size_t)k; --i)
            {
                dp[i] += dp[i - (size_t)k];
            }
        }

        last_n = (idx_t)n;               /* remember highest n computed */
    }

    return dp[(size_t)n];
}
