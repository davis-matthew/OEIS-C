/*
 * A028344  Molien series for complete weight enumerator of self-dual code
 *          over GF(5).
 * https://oeis.org/A028344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-6) - 2*a(n-7) - 3*a(n-8) +
 *            3*a(n-10) + 2*a(n-11) + a(n-12) - 2*a(n-15) - a(n-16) + a(n-18)
 *            (order‑18 linear recurrence; the first 18 terms provide the
 *            initial values)
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 936, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"


static seq_t *cache = NULL;      /* stored terms a(0)…a(k-1) */
static size_t cache_size = 0;    /* number of stored terms */

/* Initialise the cache with the first 18 terms of the sequence. */
static void init_cache(void)
{
    if (cache_size > 0)
        return;

    static const seq_t init_vals[18] = {
        1, 1, 1, 3, 4, 9, 19, 27, 41, 62,
        91, 128, 175, 231, 298, 392, 498, 617
    };

    cache = alloc_seq_array(18);
    cache_size = 18;
    for (size_t i = 0; i < 18; ++i)
        cache[i] = init_vals[i];
}

/* Ensure that the cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0)
        init_cache();

    if ((size_t)n + 1 <= cache_size)
        return;

    size_t old_size = cache_size;
    seq_t *new_cache = alloc_seq_array((size_t)n + 1);
    for (size_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];
    free_seq_array(cache);
    cache = new_cache;
    cache_size = (size_t)n + 1;

    /* Compute the new terms using the recurrence. */
    for (idx_t k = (idx_t)old_size; k <= n; ++k) {
        /* The recurrence applies only for k >= 18. */
        if (k < 18)
            continue;

        seq_t pos = cache[k - 2];                           /* a(k-2)           */
        pos += cache[k - 3]; pos += cache[k - 3];            /* +2*a(k-3)         */
        pos += cache[k - 10]; pos += cache[k - 10]; pos += cache[k - 10]; /* +3*a(k-10) */
        pos += cache[k - 11]; pos += cache[k - 11];          /* +2*a(k-11)        */
        pos += cache[k - 12];                               /* +a(k-12)          */
        pos += cache[k - 18];                               /* +a(k-18)          */

        seq_t neg = cache[k - 6];                           /* -a(k-6)           */
        neg += cache[k - 7]; neg += cache[k - 7];            /* -2*a(k-7)         */
        neg += cache[k - 8]; neg += cache[k - 8]; neg += cache[k - 8]; /* -3*a(k-8) */
        neg += cache[k - 15]; neg += cache[k - 15];          /* -2*a(k-15)        */
        neg += cache[k - 16];                               /* -a(k-16)          */

        cache[k] = pos - neg;
    }
}

/* Public function returning the n-th term of A028344. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
