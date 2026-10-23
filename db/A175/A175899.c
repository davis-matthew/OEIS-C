/*
 * A175899  a(n) = a(n-2) + a(n-3) + 2*a(n-4), with a(1) = 0, a(2) = 2, a(3) =
 *          3, a(4) = 10.
 * https://oeis.org/A175899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + 2*a(n-4), with a(1)=0, a(2)=2, a(3)=3,
 *            a(4)=10
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 98, u128 n <= 197,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;                           /* already large enough */

    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;                /* indices 0..n */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);               /* out of memory */

    /* Copy old contents */
    if (cache) {
        memcpy(new_cache, cache, old_cap * sizeof(seq_t));
        free(cache);
    }

    /* Initialize base cases if needed */
    if (new_cap > 1) new_cache[1] = (seq_t)0;
    if (new_cap > 2) new_cache[2] = (seq_t)2;
    if (new_cap > 3) new_cache[3] = (seq_t)3;
    if (new_cap > 4) new_cache[4] = (seq_t)10;

    /* Compute any new entries */
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i >= 5)
            new_cache[i] = new_cache[i-2] + new_cache[i-3] + (seq_t)2 * new_cache[i-4];
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but return 0 */
    ensure_cache(n);
    return cache[n];
}
