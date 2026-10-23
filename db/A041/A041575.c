/*
 * A041575  Denominators of continued fraction convergents to sqrt(305).
 * https://oeis.org/A041575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041575: Denominators of continued‑fraction convergents to sqrt(305)
 *
 * Recurrence:
 *   a(n) = 978*a(n-4) - a(n-8)   for n > 7
 * Initial values:
 *   a(0)=1, a(1)=2, a(2)=13, a(3)=28,
 *   a(4)=965, a(5)=1958, a(6)=12713, a(7)=27384
 *
 * The function keeps a static cache of already computed terms and expands
 * it lazily when a higher index is requested.  It works with any
 * unsigned integer type, uint8_t … uint64_t, unsigned __int128, or with
 * an arbitrary‑precision integer class (in the C++ build) because all
 * operations use only standard arithmetic operators.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms            */
    static seq_t *cache   = NULL;  /* pointer to the array          */
    static size_t cache_size = 0;  /* number of elements stored     */

    /* If the value is already in the cache, simply return it. */
    if (cache && cache_size > (size_t)n)
        return cache[n];

    /* Determine new cache size; it must be at least max(n+1,8). */
    size_t new_size = (size_t)n + 1;
    if (new_size < 8)
        new_size = 8;

    /* Allocate a new array of the required size. */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;  /* allocation failure – return 0 as a fail‑safe value */

    /* Copy any values that were already computed. */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Set the eight initial values.  They are the same for every build
       because the sequence is defined by a fixed recurrence. */
    static const seq_t init[8] = {
        (seq_t)1ULL,      /* a(0) */
        (seq_t)2ULL,      /* a(1) */
        (seq_t)13ULL,     /* a(2) */
        (seq_t)28ULL,     /* a(3) */
        (seq_t)965ULL,    /* a(4) */
        (seq_t)1958ULL,   /* a(5) */
        (seq_t)12713ULL,  /* a(6) */
        (seq_t)27384ULL   /* a(7) */
    };

    size_t i;
    for (i = 0; i < 8 && i < new_size; ++i)
        new_cache[i] = init[i];

    /* Determine from which index we need to start the recurrence.
       If the old cache already contained terms ≥ 8, we resume from
       that index.  Otherwise we start at 8. */
    size_t start = (cache_size > 8) ? cache_size : 8;

    for (i = start; i < new_size; ++i)
        new_cache[i] = (seq_t)978 * new_cache[i - 4] - new_cache[i - 8];

    /* Replace the old cache with the newly expanded one. */
    cache = new_cache;
    cache_size = new_size;

    return cache[n];
}
