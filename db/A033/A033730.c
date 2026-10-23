/*
 * A033730  Expansion of Product_{d | 18} theta_3(q^d).
 * https://oeis.org/A033730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|18} θ₃(q^d) * where θ₃(q) = 1
 *            + 2∑_{k≥1} q^(k²)
 * Range:     u8 n <= 29, u16 n <= 404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Divisors of 18. */
static const idx_t divisors[] = {1ULL, 2ULL, 3ULL, 6ULL, 9ULL, 18ULL};
static const size_t NUM_D = 6;

/* Cache of previously computed terms. */
static seq_t *cache = NULL;   /* cache[0..cache_size-1] */
static idx_t cache_size = 0;  /* number of cached terms */

/* Return the n-th term of A033730. */
seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n < cache_size)
        return cache[n];

    /* Need to compute up to index n. */
    idx_t new_size = n + 1;
    size_t sz = (size_t)new_size;

    /* Allocate two temporary arrays for the convolution. */
    seq_t *dp1 = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *dp2 = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp1 || !dp2) {
        free(dp1);
        free(dp2);
        return 0;
    }

    /* Initialize dp1: coefficient 1 at q^0, 0 elsewhere. */
    for (size_t i = 0; i < sz; ++i)
        dp1[i] = 0;
    dp1[0] = (seq_t)1;

    /* Process each divisor. */
    for (size_t di = 0; di < NUM_D; ++di) {
        idx_t d = divisors[di];

        /* Copy current coefficients to dp2. */
        for (size_t i = 0; i < sz; ++i)
            dp2[i] = dp1[i];

        /* Add contributions from non‑zero squares. */
        for (idx_t k = 1; ; ++k) {
            unsigned __int128 ek = (unsigned __int128)d * k * k;
            if (ek > n)
                break;
            size_t e = (size_t)ek;
            for (size_t m = e; m < sz; ++m) {
                dp2[m] += dp1[m - e] * (seq_t)2;
            }
        }

        /* Swap dp1 and dp2 for next iteration. */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    /* At this point dp1 holds all coefficients up to index n. */

    /* Update cache. */
    if (cache == NULL) {
        /* First time: use dp1 as the cache. */
        cache = dp1;
        cache_size = new_size;
        free(dp2);                 /* dp2 is unused now. */
        return cache[n];
    }

    if (cache_size < new_size) {
        /* Grow the cache, discarding the old one. */
        free(cache);
        cache = dp1;
        cache_size = new_size;
        free(dp2);
        return cache[n];
    }

    /* Cache is large enough. Copy the newly computed values. */
    for (size_t i = 0; i < sz; ++i)
        cache[i] = dp1[i];
    free(dp1);
    free(dp2);
    return cache[n];
}
