/*
 * A168143  a(17)=37; for n>=17, a(n)=3n-14 if gcd(n,a(n-1))>1 and all prime
 *          divisors of n more than 17; a(n)=a(n-1)+1, otherwise.
 * https://oeis.org/A168143
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n-14 when gcd(n,a(n-1))>1 and all prime divisors of n
 *            are >17, otherwise a(n) = a(n-1)+1; with a(17)=37. Offset: 17
 *            (The sequence is computed iteratively and memoised for arbitrary
 *            call order.)
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, prime_divs_gt17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/prime_divs_gt17.h"

/* Cached values of a(n).  cache[i] holds a(i) for i >= 17; indices <17
   are unused.  cache_size == highest computed index + 1. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of stored entries (max index + 1) */
static idx_t  cache_cap   = 0;   /* allocated capacity */

/* Initialise cache with base value a(17)=37. */
static void init_cache(void)
{
    /* Allocate a modest initial capacity. */
    cache_cap = 64;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) return;               /* out of memory – caller will handle */
    /* Fill unused entries. */
    for (idx_t i = 0; i < 17; ++i) cache[i] = 0;
    cache[17] = 37;
    cache_size = 18;                  /* indices 0..17 inclusive */
}

/* Ensure the cache has room for at least 'need' indices. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 17) return 0;            /* behaviour below offset is irrelevant */

    /* Initialise cache lazily. */
    if (!cache) init_cache();

    if (n < cache_size) return cache[n];

    /* Expand cache to cover n. */
    ensure_capacity(n + 1);

    /* Compute successive terms from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g    = gcd((seq_t)i, prev);
        bool  cond = (g > 1) && prime_divs_gt17(i);
        cache[i]   = cond ? (3 * (seq_t)i - 14) : prev + 1;
    }
    cache_size = n + 1;
    return cache[n];
}
