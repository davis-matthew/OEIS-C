/*
 * A166914  a(n) = 20*a(n-1) - 64*a(n-2) for n > 1; a(0) = 21, a(1) = 340.
 * https://oeis.org/A166914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 21, a(1) = 340, a(n) = 20*a(n-1) - 64*a(n-2) (n > 1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms */
static size_t cache_cap = 0;         /* capacity of allocated array */
static size_t cache_size = 0;        /* number of elements currently stored */

/* Grow the cache to be able to store indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= (size_t)n) {
        new_cap <<= 1;
    }

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();        /* fatal error if memory allocation fails */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Fast return for trivial cases (if cache empty this will still work). */
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)340;

    /* Make sure we have space for all required indices. */
    ensure_cache_capacity(n);

    /* Compute (and store) missing terms up to n. */
    size_t i;
    for (i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) val = (seq_t)21;
        else if (i == 1) val = (seq_t)340;
        else {
            /* Perform recurrence: 20 * a(i-1) - 64 * a(i-2) */
            val = (seq_t)20 * cache[i-1] - (seq_t)64 * cache[i-2];
        }
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
