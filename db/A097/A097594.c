/*
 * A097594  a(n) = (a(n-1) mod a(n-2)) + a(n-2), a(0) = 3, a(1) = 2.
 * https://oeis.org/A097594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1) mod a(n-2)) + a(n-2), a(0)=2, a(1)=5
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 86, u64 n <= 178,
 *            u128 n <= 364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence values.  The array is grown on demand. */
static seq_t *cache = NULL;         /* array of a(k) */
static idx_t cache_size = 0;        /* number of stored elements */
static idx_t cache_cap   = 0;        /* current capacity */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    /* Grow capacity: at least 8, double each time, but not less than needed. */
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed)
        new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return a(n) according to the recurrence. */
seq_t a(idx_t n)
{
    /* Initialize the cache with the first two terms if empty. */
    if (cache_size == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    /* Extend the cache up to n if necessary. */
    while (cache_size <= n) {
        /* Compute next term from the last two cached terms. */
        seq_t a_prev      = cache[cache_size - 1];
        seq_t a_prevprev  = cache[cache_size - 2];
        seq_t term = (a_prev % a_prevprev) + a_prevprev;

        ensure_cache_cap(cache_size + 1);
        cache[cache_size] = term;
        cache_size++;
    }

    return cache[n];
}
