/*
 * A041217  Denominators of continued fraction convergents to sqrt(119).
 * https://oeis.org/A041217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 240*a(n-4) - a(n-8) Recurrence order: 8 Initial values (n
 *            = 0..7): a(0) = 1, a(1) = 1, a(2) = 10, a(3) = 11, a(4) = 230,
 *            a(5) = 241, a(6) = 2399, a(7) = 2640
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static void extend_cache(idx_t n, seq_t **p_cache, size_t *cap)
{
    if ((size_t)n + 1 > *cap) {
        size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *new_cache = reinterpret_cast<seq_t*>(
            realloc(*p_cache, new_cap * sizeof(seq_t)));
#else
        seq_t *new_cache = (seq_t*)realloc(*p_cache, new_cap * sizeof(seq_t));
#endif
        if (!new_cache) {          /* allocation failed – leave cache unchanged */
            return;
        }
        *p_cache = new_cache;
        *cap     = new_cap;
    }
}

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1,    /* a(0) */
        (seq_t)1,    /* a(1) */
        (seq_t)10,   /* a(2) */
        (seq_t)11,   /* a(3) */
        (seq_t)230,  /* a(4) */
        (seq_t)241,  /* a(5) */
        (seq_t)2399, /* a(6) */
        (seq_t)2640  /* a(7) */
    };

    /* Return small indices directly */
    if (n < 8) {
        return init[n];
    }

    /* Lazy cache that grows on demand */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    extend_cache(n, &cache, &cap);

    /* Initialise the first eight terms in the cache */
    for (idx_t i = 0; i < 8; ++i) {
        cache[i] = init[i];
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        cache[i] = (seq_t)240 * cache[i - 4] - cache[i - 8];
    }

    return cache[n];
}
