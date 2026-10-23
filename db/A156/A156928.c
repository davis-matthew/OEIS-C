/*
 * A156928  G.f. of the z^1 coefficients of the FP1 in the second column of
 *          the A156921 matrix.
 * https://oeis.org/A156928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4) + 2 (n >= 6)
 *            Initial values: a(2) = 1, a(3) = 7, a(4) = 28, a(5) = 86
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;     /* cache[i] holds a(i) */
static idx_t  cache_size = 0;   /* current allocated size (indices 0 .. cache_size-1) */

/* initialise cache with the first six elements (a(0) and a(1) are unused) */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_size = 6;                     /* indices 0..5 */
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    if (!cache)
        exit(1);                         /* out of memory */
    cache[0] = 0;                         /* unused */
    cache[1] = 0;                         /* unused */
    cache[2] = 1;                         /* a(2) */
    cache[3] = 7;                         /* a(3) */
    cache[4] = 28;                        /* a(4) */
    cache[5] = 86;                        /* a(5) */
}

/* extend the cache up to at least index n */
static void extend_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;                     /* indices 0 .. n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache)
        exit(1);                                 /* out of memory */
    cache = new_cache;
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 5)      /* already initialised in init_cache */
            continue;
        /* recurrence: a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4) + 2 */
        cache[i] = 5 * cache[i-1]
                 - 9 * cache[i-2]
                 + 7 * cache[i-3]
                 - 2 * cache[i-4]
                 + 2;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 2)          /* values below the offset are irrelevant */
        return 0;
    init_cache();
    if (n >= cache_size)
        extend_cache(n);
    return cache[n];
}
