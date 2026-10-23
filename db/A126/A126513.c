/*
 * A126513  Number of base 18 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑18 n‑digit numbers whose adjacent digits
 *            differ by ≤ 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The count is computed by dynamic programming: dp[d] = number of length‑len strings
   ending in digit d.  Transition: dp_next[d] = Σ_{|k−d|≤4} dp[k].  The total a(n)
   is the sum of dp values for length n.  The sequence is memoised so repeated calls
   are fast. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Early return for the trivial case */
    if (n == 0)
    {
        return (seq_t)1;
    }

    static seq_t *cache      = NULL;          /* memoised a(k) values   */
    static size_t cache_size = 0;             /* allocated cache size   */
    static idx_t  computed_max = 0;           /* largest n computed     */
    static seq_t  dp[18];                     /* number of strings of
                                                 current length ending
                                                 in each digit          */
    static seq_t  next_dp[18];                /* helper for next length */
    static idx_t  current_len = 0;            /* current length of dp[] */

    /* Initialise data structures on first use */
    if (cache == NULL)
    {
        cache_size = 2U;                          /* stores a(0) and a(1)   */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure      */
        cache[0] = (seq_t)1;                     /* a(0) */
        cache[1] = (seq_t)18;                    /* a(1) */
        for (int d = 0; d < 18; ++d) dp[d] = (seq_t)1;  /* length 1 strings      */
        current_len = 1;
        computed_max = 1;
    }

    /* Ensure enough space in cache for all requested values */
    if ((idx_t)n >= (idx_t)cache_size)
    {
        size_t new_size = cache_size * 2U;
        while (new_size <= (size_t)n) new_size *= 2U;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure      */
        cache = tmp;
        cache_size = new_size;
    }

    /* Answer already computed? */
    if (n <= computed_max)
        return cache[n];

    /* Extend computation up to n */
    for (idx_t len = current_len + 1; len <= n; ++len)
    {
        /* Transition to next length */
        for (int d = 0; d < 18; ++d)
        {
            seq_t sum = 0;
            int low  = d - 4;
            if (low < 0) low = 0;
            int high = d + 4;
            if (high > 17) high = 17;
            for (int k = low; k <= high; ++k)
                sum += dp[k];
            next_dp[d] = sum;
        }

        /* Compute total for this length and store */
        seq_t total = 0;
        for (int d = 0; d < 18; ++d)
            total += next_dp[d];
        cache[len] = total;

        /* Prepare for next iteration */
        for (int d = 0; d < 18; ++d)
            dp[d] = next_dp[d];
    }

    current_len = n;
    computed_max = n;
    return cache[n];
}
