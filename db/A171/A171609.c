/*
 * A171609  a(2*n) = (n+1)*a(n), a(2*n+1) = (n+1)*a(n+1), with a(1) = 1.
 * https://oeis.org/A171609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence: a(1) = 1 a(2*n) = (n+1) * a(n) a(2*n+1) = (n+1) *
 *            a(n+1)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Global cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to index 'n' inclusive */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Out of memory – terminate gracefully */
        abort();
    }
    /* Initialise newly allocated space to zero */
    memset(new_cache + cache_cap + 1, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 */

    ensure_cache(n);

    /* If already computed, return it */
    if (cache[n] != 0)
        return cache[n];

    /* Base case */
    if (n == 1) {
        cache[1] = (seq_t)1;
        return cache[1];
    }

    seq_t result;
    if ((n & 1) == 0) {            /* even */
        idx_t m = n >> 1;          /* n / 2 */
        ensure_cache(m);
        seq_t a_m = cache[m] ? cache[m] : a(m);
        result = (seq_t)(m + 1) * a_m;
    } else {                       /* odd */
        idx_t m = (n - 1) >> 1;    /* (n - 1) / 2 */
        ensure_cache(m + 1);
        seq_t a_m1 = cache[m + 1] ? cache[m + 1] : a(m + 1);
        result = (seq_t)(m + 1) * a_m1;
    }

    cache[n] = result;
    return result;
}
