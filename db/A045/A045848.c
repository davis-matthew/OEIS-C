/*
 * A045848  Number of nonnegative solutions of x1^2 + x2^2 + ... + x6^2 = n.
 * https://oeis.org/A045848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative solutions of x1^2 + x2^2 + x3^2 +
 *            x4^2 + x5^2 + x6^2 = n
 * Range:     u8 n <= 17, u16 n <= 414, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by a six‑fold convolution of the
   generating function 1 + q + q^4 + q^9 + …, i.e. by dynamic
   programming.  The implementation keeps a cache so that
   subsequent calls are O(1) after the first computation up to the
   maximum requested n.                                            */

#include <stdlib.h>

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t k = 0;
    while ((k + 1) <= n / (k + 1))
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* a(0)…a(cache_size-1) */
    static idx_t cache_size = 0;  /* number of cached elements */

    if (n + 1 > cache_size) {
        /* compute a(0)..a(n) and extend the cache */

        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*) calloc(new_size, sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure – unlikely */

        /* dynamic‑programming arrays for the 6 convolutions */
        seq_t *dp = (seq_t*) calloc(new_size, sizeof(seq_t));
        seq_t *next;
        if (!dp) { free(new_cache); return 0; }

        dp[0] = 1;   /* 0 variables give sum 0 */

        idx_t kmax = floor_sqrt_u64(n);

        for (idx_t var = 0; var < 6; ++var) {
            next = (seq_t*) calloc(new_size, sizeof(seq_t));
            if (!next) { free(dp); free(new_cache); return 0; }

            for (idx_t k = 0; k <= kmax; ++k) {
                idx_t sq = k * k;
                for (idx_t i = 0; i + sq <= n; ++i)
                    next[i + sq] += dp[i];
            }

            free(dp);
            dp = next;
        }

        /* copy computed values into the cache */
        for (idx_t i = 0; i < new_size; ++i)
            new_cache[i] = dp[i];

        free(dp);
        free(cache);          /* safe even if cache == NULL */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
