/*
 * A073937  a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4), a(0)=4, a(1)=1, a(2)=-1,
 *          a(3)=1.
 * https://oeis.org/A073937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4), a(0)=4, a(1)=1,
 *            a(2)=-1, a(3)=1
 * Range:     i8 n <= 16, i16 n <= 41, i32 n <= 84, i64 n <= 171,
 *            i128 n <= 345, big any n
 * Validated: standard, pass, 67 terms through n = 8041 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper to ensure the cache holds values up to index 'n'.
   The cache is a static array of seq_t that grows as needed. */
static void ensure_values(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of elements currently stored */
    static idx_t cap   = 0;   /* allocated capacity */

    if (cache == NULL) { /* first call: allocate and store the 4 base terms */
        cap = 16;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)1;
        size = 4;
    }

    /* extend cache to include value n */
    if (n < size) return;              /* already computed */

    if (n >= cap) {
        /* grow; double until big enough */
        while (cap <= n) cap <<= 1;
        cache = mem_resize(seq_t, cache, cap);
    }

    for (idx_t i = size; i <= n; ++i) {
        cache[i] = cache[i-1] - cache[i-2] + cache[i-3] + cache[i-4];
    }
    size = n + 1;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)1;

    /* Ensure cache has the value */
    static seq_t *cache = NULL;
    static idx_t size = 0;
    static idx_t cap  = 0;

    if (cache == NULL) {
        /* initialise cache with base values */
        cap = 16;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)1;
        size = 4;
    }

    if (n < size) return cache[n];

    /* grow if necessary */
    if (n >= cap) {
        while (cap <= n) cap <<= 1;
        cache = mem_resize(seq_t, cache, cap);
    }

    /* compute missing terms */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = cache[i-1] - cache[i-2] + cache[i-3] + cache[i-4];
    }
    size = n + 1;

    return cache[n];
}
