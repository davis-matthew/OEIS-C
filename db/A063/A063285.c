/*
 * A063285  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          12 ).
 * https://oeis.org/A063285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8);
 *            Initial values: a(2)=-1, a(3)=0, a(4)=3, a(5)=5, a(6)=5, a(7)=8,
 *            a(8)=8, a(9)=14, a(10)=11, a(11)=17
 * Range:     i8 n <= 62, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  max_computed = 0;
static int    initialized  = 0;

/* Initialise the cache with the first known terms. */
static void init_cache(void)
{
    if (!initialized) {
        cache_cap = 11;                /* indices 0 .. 10 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* Allocation failed – fall back to zero value. */
            cache_cap = 0;
            initialized = 1;
            return;
        }
        cache[0] = cache[1] = (seq_t)0;   /* dummy values for n < 2 */
        cache[2] = (seq_t)(-1);
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)5;
        cache[7] = (seq_t)8;
        cache[8] = (seq_t)8;
        cache[9] = (seq_t)14;
        cache[10] = (seq_t)11;
        max_computed = 10;
        initialized = 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)         /* behaviour for n < 2 is irrelevant */
        return (seq_t)0;

    init_cache();
    if (!initialized)         /* allocation failure */
        return (seq_t)0;

    size_t idx = (size_t)n;

    /* Extend the cache if necessary. */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)          /* allocation failure */
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return a pre‑computed value if available. */
    if (idx <= max_computed)
        return cache[idx];

    /* Compute missing terms using the linear recurrence. */
    for (size_t i = max_computed + 1; i <= idx; ++i) {
        seq_t term = cache[i - 1];
        term = term - cache[i - 3];
        term = term + ((seq_t)2) * cache[i - 4];
        term = term - cache[i - 5];
        term = term + cache[i - 7];
        term = term - cache[i - 8];
        cache[i] = term;
    }

    max_computed = idx;
    return cache[idx];
}
