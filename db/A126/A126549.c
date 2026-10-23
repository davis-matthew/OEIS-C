/*
 * A126549  Number of base 28 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = number of base‑28 n‑digit numbers whose adjacent digits differ by at most 5.
    DP recurrence: a(0)=1, a(1)=28. For k>1 let dp[d] be the number of
    length‑k strings ending with digit d, then dp_next[d] = Σ_{|d-p|≤5} dp[p].
    a(k) is the sum over all d of dp_next[d]. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* cache[0..cache_size] = a(0..cache_size) */
    static idx_t  cache_size = 0;
    static seq_t dp[28];              /* dp for the current length */
    static seq_t newdp[28];

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1; /* a(0) */

        /* dp for length 1: one string for each digit */
        for (int d = 0; d < 28; ++d)
            dp[d] = (seq_t)1;
        seq_t sum = (seq_t)0;
        for (int d = 0; d < 28; ++d)
            sum += dp[d];
        cache[1] = sum;          /* a(1) = 28 */
        cache_size = 1;
    }

    /* Return cached value if available */
    if (n <= cache_size)
        return cache[n];

    /* Expand cache to accommodate a(n) */
    size_t newlen = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (tmp == NULL)               /* allocation failure – unlikely in tests */
        return (seq_t)0;
    cache = tmp;

    /* Compute values for lengths cache_size+1 … n */
    for (idx_t len = cache_size; len < n; ++len) {
        /* dp currently holds counts for strings of length = len */
        for (int dest = 0; dest < 28; ++dest) {
            newdp[dest] = (seq_t)0;
            int lo = dest - 5;
            if (lo < 0) lo = 0;
            int hi = dest + 5;
            if (hi > 27) hi = 27;
            for (int src = lo; src <= hi; ++src)
                newdp[dest] += dp[src];
        }

        /* Sum of newdp gives a(len+1) */
        seq_t sum = (seq_t)0;
        for (int d = 0; d < 28; ++d)
            sum += newdp[d];
        cache[len + 1] = sum;

        /* Prepare for next iteration */
        for (int d = 0; d < 28; ++d)
            dp[d] = newdp[d];
    }

    cache_size = n;
    return cache[n];
}
