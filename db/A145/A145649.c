/*
 * A145649  Characteristic function of the lucky numbers.
 * https://oeis.org/A145649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a lucky number (A000959), 0 otherwise
 * Range:     u8 n <= 9816, u16 n <= 9816, u32 n <= 9816, u64 n <= 9816,
 *            u128 n <= 9816, big any n
 * Helpers:   build_lucky_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation sieves the lucky numbers up to the largest n requested. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_lucky_up_to.h"

/* Forward declaration of the helper */
static void build_lucky_up_to(idx_t max_n, bool *is_lucky);

/* Cached sieve data */
static idx_t cache_size = 0;
static bool *lucky_cache = NULL;

/* Return 1 if n is lucky, 0 otherwise. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Rebuild or extend the sieve if necessary */
    if (n > cache_size) {
        /* Reallocate boolean mask */
        bool *new_cache = (bool *)realloc(lucky_cache,
                            (n + 1) * sizeof(bool));
        if (!new_cache) {                /* allocation failure fallback */
            if (lucky_cache) free(lucky_cache);
            lucky_cache = NULL;           /* no cache, just return */
            return (seq_t)0;
        }
        lucky_cache = new_cache;
        for (idx_t i = 1; i <= n; ++i)
            lucky_cache[i] = false;      /* initialise to false */

        build_lucky_up_to(n, lucky_cache);
        cache_size = n;
    }

    return lucky_cache[n] ? (seq_t)1 : (seq_t)0;
}
