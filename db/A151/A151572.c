/*
 * A151572  a(0)=0, a(1)=1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j <
 *            2^i
 * Range:     u8 n <= 120, u16 n <= 4085, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Local cache using static storage. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure the cache can hold at least (n+1) entries. */
static void assure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* abort on allocation failure */

    /* Copy existing data and zero new entries. */
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Prepare cache for current index. */
    assure_cache(n);

    /* If already computed, return it. */
    if (cache[n] != 0) return cache[n];

    /* Compute the largest power of two <= n. */
    idx_t i = 63 - __builtin_clzll(n);          /* floor(log2(n))   */
    idx_t base = ((idx_t)1) << i;              /* 2^i             */
    idx_t j = n - base;                        /* j < 2^i         */

    /* Recursively compute a(j) and a(j+1). */
    seq_t val_j   = a(j);
    seq_t val_j1  = a(j + 1);

    /* a(n) = a(j) + 2*a(j+1).  Use addition to avoid
       shifting on arbitrary-precision types. */
    seq_t result = val_j + val_j1 + val_j1;

    /* Store and return. */
    cache[n] = result;
    return result;
}
