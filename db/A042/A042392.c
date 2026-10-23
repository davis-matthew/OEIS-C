/*
 * A042392  Numerators of continued fraction convergents to sqrt(723).
 * https://oeis.org/A042392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 484*a(n−4) - a(n−8) Initial values: a(0)=26, a(1)=27,
 *            a(2)=215, a(3)=242, a(4)=12799, a(5)=13041, a(6)=104086,
 *            a(7)=117127 (subsequent terms are computed via the recurrence)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer literals for the first eight terms.
   They are stored as 64‑bit values to avoid truncation warnings
   when seq_t may be smaller (e.g. uint8_t). */
static const uint64_t init_vals[8] = {
    26ULL,   /* a(0) */
    27ULL,   /* a(1) */
    215ULL,  /* a(2) */
    242ULL,  /* a(3) */
    12799ULL,/* a(4) */
    13041ULL,/* a(5) */
    104086ULL,/* a(6) */
    117127ULL /* a(7) */
};

seq_t a(idx_t n)
{
    /* Cached computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Return one of the initial values directly. */
    if (n < 8)
        return (seq_t) init_vals[n];

    /* Lazy allocation of cache on first use. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();  /* allocation failure */
        /* Initialise the first eight entries. */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = (seq_t) init_vals[i];
        cache_size = 8;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if (n >= cache_size) {
        /* Double the capacity until it can hold the requested index. */
        while (cache_cap <= n) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap <= cache_cap)   /* overflow guard */
                new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from the current end up to n inclusive. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t part = cache[i - 4] * (seq_t)484;
            part -= cache[i - 8];
            cache[i] = part;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
