/*
 * A037607  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3.
 * https://oeis.org/A037607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7 * a(n‑1) + ((n-1)%3 + 1), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑7 representation where the digits cycle 1,2,3,1,2,3,… */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] holds a(1) */
static idx_t cache_len = 0;     /* number of terms stored in cache */

seq_t a(idx_t n)
{
    /* n is at least 1 by specification. */
    if (n == 0) return 0;      /* safeguard for unexpected calls */

    /* Initialise cache for the first term */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed */
        cache[0] = 1;                        /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to index n if necessary */
    if (n > cache_len) {
        /* Reallocate to hold n terms (index 0 … n-1) */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return 0;           /* allocation failed */
        cache = new_cache;

        /* Compute new terms iteratively */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 2];     /* a(i-1) */
            seq_t digit  = (seq_t)(((i - 1) % 3) + 1); /* digit at position i */
            cache[i - 1] = prev * 7 + digit; /* a(i) = 7*a(i-1) + digit */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
