/*
 * A215694  a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) with a(0)=1, a(1)=2, a(2)=7.
 * https://oeis.org/A215694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) with initial values a(0)=1,
 *            a(1)=2, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static idx_t cache_size = 0;     /* number of stored entries */

/* Returns the n‑th term of A215694, 0‑based. */
seq_t a(idx_t n)
{
    /* Handle the known base cases immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    /* Initialise the cache if this is the first call. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
    }

    /* If the requested index is beyond what we've computed, extend the cache. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out of memory */
        cache = tmp;

        /* Compute new entries using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t A1 = cache[i-1];
            seq_t A2 = cache[i-2];
            seq_t A3 = cache[i-3];
            cache[i] = (seq_t)(5 * A1 - 6 * A2 + A3);
        }
        cache_size = new_size;
    }

    return cache[n];
}
