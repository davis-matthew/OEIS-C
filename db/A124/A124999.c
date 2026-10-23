/*
 * A124999  Number of base 5 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A124999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 3*a(n-3) for n>3 a(0)=1, a(1)=5,
 *            a(2)=23, a(3)=101
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache    = NULL;          /* array of previously computed values */
static size_t cache_size  = 0;          /* allocated array length      */
static idx_t  last_computed = 3;        /* highest index that has been filled correctly */

/* make sure the cache is large enough to hold index n   */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        /* first allocation, at least enough for indices 0..3  */
        size_t init_size = 4;
        while (init_size <= (size_t)n) init_size <<= 1;
#ifdef __cplusplus
        cache = new seq_t[init_size];
#else
        cache = malloc(sizeof(seq_t) * init_size);
#endif
        cache_size = init_size;
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 23;
        cache[3] = 101;
        last_computed = 3;
    }
    else if (cache_size <= (size_t)n) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size];
        /* copy existing values */
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        delete [] cache;
#else
        seq_t *new_cache = realloc(cache, sizeof(seq_t) * new_size);
#endif
        cache = new_cache;
        cache_size = new_size;
        /* existing values remain, no need to re‑initialise */
    }
}

seq_t a(idx_t n)
{
    /* handle special cases directly to avoid loop  */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 23;
    if (n == 3) return 101;

    ensure_cache_size(n);

    if (n <= last_computed) return cache[n];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - cache[i-2] - 3 * cache[i-3];
    }
    last_computed = n;
    return cache[n];
}
