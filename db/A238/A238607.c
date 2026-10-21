/*
 * A238607  Number of partitions p of 2n such that n - (number of parts of p)
 *          is a part of p.
 * https://oeis.org/A238607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1}^{n-2} p(n + k + 1, k) where p(N, k) is the
 *            number of partitions of N into exactly k parts. Using the
 *            bijection p(N, k) = number of partitions of (N - k) into at most
 *            k parts, we compute directly with the classic coin‑change DP
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 89, u64 n <= 327,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *a_cache = NULL;
static idx_t a_cached_up_to = 0;

static seq_t compute_a_upto(idx_t n) {
    /* Allocate cache for a[0..n] */
    size_t cache_size = (size_t)n + 1;          /* index n is needed */
    seq_t *new_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;            /* fallback */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = (seq_t)0;                /* initialize to zero */

    if (n < 3) {                                /* a(1)=a(2)=0 */
        /* free previous cache if any */
        if (a_cache) free(a_cache);
        a_cache = new_cache;
        a_cached_up_to = n;
        return a_cache[n];
    }

    /* DP array: dp[0..n+1] for partitions with parts up to current k */
    size_t dp_size = (size_t)n + 2;             /* indices 0 .. n+1 inclusive */
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp) {
        free(new_cache);
        return (seq_t)0;
    }
    for (size_t i = 0; i < dp_size; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                           /* empty partition */

    /* Main loops: for each part size k */
    for (idx_t k = 1; k <= n - 2; ++k) {
        /* update dp for new part size k (unbounded coin change) */
        for (idx_t s = k; s <= n + 1; ++s)
            dp[s] += dp[s - k];

        /* accumulate contributions to a[n] for n >= k+2 */
        for (idx_t s = k + 3; s <= n + 1; ++s)
            new_cache[s - 1] += dp[s];
    }

    /* Free old cache if any */
    if (a_cache) free(a_cache);
    a_cache = new_cache;
    a_cached_up_to = n;

    /* Free temporary dp */
    free(dp);

    return a_cache[n];
}

seq_t a(idx_t n) {
    if (n > a_cached_up_to) {
        return compute_a_upto(n);
    } else {
        return a_cache[n];
    }
}
