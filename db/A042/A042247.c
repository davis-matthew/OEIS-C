/*
 * A042247  Denominators of continued fraction convergents to sqrt(649).
 * https://oeis.org/A042247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Denominators of convergents to sqrt(649)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t: unsigned integer type selected by the build (uint8_t … unsigned __int128 or big integer class). */
/* idx_t: unsigned 64‑bit integer type (uint64_t). */

seq_t a(idx_t n)
{
    /* Periodic partial quotients of sqrt(649) after the initial term a0 = 25. */
    const idx_t period_len = 30;
    static const seq_t period[30] = {
        2, 9, 1, 2, 3, 1, 1, 2, 1, 4,
        1, 16, 6, 3, 4, 3, 6, 16, 1, 4,
        1, 2, 1, 1, 3, 2, 1, 9, 2, 50
    };

    static seq_t *cache = NULL;
    static idx_t cached_len = 0;        /* number of terms stored (cache[0] .. cache[cached_len-1]) */
    static size_t cache_cap = 0;        /* allocated capacity of cache */

    /* Handle outermost request for term 0 */
    if (n == 0) return 1;

    /* Allocate cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */
        cache[0] = 1;                   /* a(0) = 1 */
        cached_len = 1;
    }

    /* Grow cache if necessary */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        /* Determine partial quotient a_i */
        seq_t a_i;
        if (i == 0) {
            a_i = 25;                  /* a0 */
        } else {
            a_i = period[(i - 1) % period_len]; /* periodic part */
        }

        /* Recurrence: k_i = a_i * k_{i-1} + k_{i-2}  with k_{-1} = 0, k_0 = 1 */
        seq_t prev    = cache[i - 1];
        seq_t prevprev = (i >= 2) ? cache[i - 2] : 0;
        seq_t new_q   = a_i * prev + prevprev;

        cache[i] = new_q;
    }

    cached_len = n + 1;
    return cache[n];
}
