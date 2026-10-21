/*
 * A239427  Numbers such that additive and multiplicative persistences
 *          coincide.
 * https://oeis.org/A239427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239427: numbers whose additive persistence equals
   their multiplicative persistence.  The function searches
   for terms linearly, caching results so that calls with
   arbitrary n (≥1) are fast. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"

/* Cache of already found terms. */
static seq_t *cache = NULL;       /* array of terms             */
static idx_t  cache_size = 0;     /* number of terms stored    */
static idx_t  cache_cap   = 0;     /* allocated capacity        */
static seq_t  next_candidate = 0;  /* next number to test       */

/* Compute additive persistence of x. */
static int additive_persistence(seq_t x)
{
    int cnt = 0;
    while (x >= (seq_t)10) {
        x = digit_sum(x);
        ++cnt;
    }
    return cnt;
}

/* Compute multiplicative persistence of x. */
static int multiplicative_persistence(seq_t x)
{
    int cnt = 0;
    while (x >= (seq_t)10) {
        x = digit_product(x);
        ++cnt;
    }
    return cnt;
}

/* Grow the cache to at least `new_cap` terms. */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        return;  /* OOM: caller will recover */
    if (cache != NULL) {
        memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1; guard for defensive use */
        return (seq_t)0;

    /* Ensure the cache contains at least n terms. */
    while (cache_size < n) {
        seq_t val = next_candidate;
        if (additive_persistence(val) == multiplicative_persistence(val)) {
            if (cache_size == cache_cap)
                grow_cache(cache_cap == (idx_t)0 ? (idx_t)128 : cache_cap * (idx_t)2);
            cache[cache_size++] = val;
        }
        ++next_candidate;
    }

    return cache[n - (idx_t)1];
}
