/*
 * A077833  Expansion of 1/(1-2*x-3*x^2-2*x^3).
 * https://oeis.org/A077833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 2*a(n-3) Recurrence holds for n ≥ 3
 *            with initial values a(0) = 1, a(1) = 2, a(2) = 7. This
 *            implements the expansion of 1/(1‑2x‑3x²‑2x³)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t  cache_size = 0; /* number of computed entries */

/* Extend the cache so that it contains at least n+1 elements. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;            /* already sufficient */

    idx_t new_size = n + 1;
    if (new_size < 3) new_size = 3;        /* at least hold first three */

    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (cache == NULL) exit(1);           /* memory allocation failure */

    /* initialise base cases if this is the first allocation */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
    }

    /* compute new values using the recurrence */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 3) continue;              /* base terms already set */
        cache[i] = (seq_t)2 * cache[i-1]
                 + (seq_t)3 * cache[i-2]
                 + (seq_t)2 * cache[i-3];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
