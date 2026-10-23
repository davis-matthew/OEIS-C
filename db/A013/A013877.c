/*
 * A013877  a(n) = 15^(5*n + 4).
 * https://oeis.org/A013877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Memoized cache of computed terms.  */
static seq_t *cache = NULL;          /* array of terms a(0)..a(cache_len-1)  */
static idx_t cache_len = 0;          /* number of terms currently stored  */
static size_t cache_cap = 0;         /* allocated capacity of cache         */

/* 15^5, used to go from a(n) to a(n+1).  */
static seq_t base5;

/* One-time initialization of cache and base5.  */
static void init_cache(void)
{
    if (cache != NULL)
        return; /* already initialized */

    /* Compute base5 = 15^5 once. */
    base5 = big_pow((seq_t)15, (seq_t)5);

    /* Allocate initial storage.  */
    cache_cap = 64;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache)
        return; /* allocation failed – program may crash on first call */

    /* Compute a(0) = 15^4. */
    cache[0] = big_pow((seq_t)15, (seq_t)4);
    cache_len = 1;
}

/* Ensure that cache has room for at least n+1 terms and that term n is computed. */
static void ensure_up_to(idx_t n)
{
    if (n < cache_len)
        return; /* already available */

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache)
            return; /* realloc failed – leave cache unchanged */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Build missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = seq_mul(cache[i - 1], base5);
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    init_cache();          /* guarantee cache is ready */
    ensure_up_to(n);       /* compute term if needed */
    return cache[n];
}
