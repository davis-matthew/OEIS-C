/*
 * A139818  Squares of Jacobsthal numbers.
 * https://oeis.org/A139818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 8*a(n-3), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stored values */
static idx_t  cache_size = 0;      /* array capacity (in terms of index+1) */
static idx_t  last_computed = 0;   /* index of the last computed value */

/* Ensure that the cache can hold values up to index `need`.  Initialises
 * the first three terms if this is the first call. */
static void
ensure_cache(idx_t need)
{
    if (cache == NULL) {
        /* Allocate at least 3 slots, but expand to `need+1` if larger. */
        size_t init_cap = 3;
        if (need + 1 > init_cap) init_cap = need + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_size = init_cap;
        last_computed = 2;
    } else if (cache_size <= need) {
        /* Expand the cache to at least `need+1` entries. */
        size_t new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_cap;
        /* new entries are uninitialised; they will be filled in the loop below. */
    }
}

seq_t a(idx_t n)
{
    /* 1. Small indices – return the known base values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* 2. Ensure that the cache contains the first three terms. */
    ensure_cache(n);

    /* 3. Return from cache if already computed. */
    if (n <= last_computed) return cache[n];

    /* 4. Compute missing terms iteratively using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term =
            (cache[i - 1] * 3) + (cache[i - 2] * 6) -
            (cache[i - 3] * 8);
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
