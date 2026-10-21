/*
 * A218355  Number of partitions into distinct parts where all differences
 *          between consecutive parts are odd and the minimal part is even.
 * https://oeis.org/A218355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 143, u32 n <= 416, u64 n <= 1356,
 *            u128 n <= 64, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A218355
    Number of partitions into distinct parts where
    all differences between consecutive parts are odd
    and the minimal part is even.
    Computation is based on the generating function:
    sum_{m>=0} x^{(m+1)(m+4)/2} / prod_{k=1}^{m+1} (1 - x^{2k}) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        /* allocate new cache */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;

        /* dynamic programming on even part sizes */
        seq_t *dp = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!dp) {
            free(new_cache);
            return (seq_t)0;
        }
        dp[0] = (seq_t)1;          /* empty partition */
        new_cache[0] = (seq_t)1;   /* a(0) = 1 */

        idx_t m = 0;
        while (1) {
            /* minimal sum for partitions with m+1 parts */
            idx_t S = ((m + 1) * (m + 4)) / 2;
            if (S > n) break;      /* no more contributions */

            idx_t v = 2 * (m + 1);  /* new even part size allowed */

            /* update dp with unlimited copies of part of size v */
            for (size_t s = (size_t)v; s <= (size_t)n; ++s)
                dp[s] += dp[s - v];

            /* add contributions of this m to the answer */
            size_t max_s = (size_t)(n - S);
            for (size_t s = 0; s <= max_s; ++s)
                new_cache[s + (size_t)S] += dp[s];

            ++m;
        }

        free(dp);
        free(cache);          /* free old cache if any */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
