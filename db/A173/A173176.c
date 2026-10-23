/*
 * A173176  Duplicate of A172057.
 * https://oeis.org/A173176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the larger member of the n‑th twin prime pair (p, p+2)
 * Range:     i8 n <= 9, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;      /* Cached sequence values */
static size_t cache_size = 0;    /* Number of stored values    */
static size_t cache_cap = 0;     /* Current capacity of cache  */
static idx_t next_candidate = 5; /* Next odd number to test as p */

/* Ensure that `cache` can hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();          /* Out of memory */

    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Undefined for n < 1; return 0 safely */

    /* Grow the cache until it contains at least `n` values. */
    while (cache_size < (size_t)n) {
        /* Find the next twin prime pair (p, p+2). */
        while (!(is_prime(next_candidate) && is_prime(next_candidate + 2))) {
            next_candidate += 2;   /* Skip even numbers. */
        }

        /* Store the upper member of the twin prime pair. */
        ensure_capacity(cache_size + 1);
        cache[cache_size++] = (seq_t)(next_candidate + 2);

        /* Advance to the next odd number after the current pair. */
        next_candidate += 4;
    }

    return cache[n - 1];
}
