/*
 * A171688  Twin primes > 3.
 * https://oeis.org/A171688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A001097(n+1) (twin primes greater than 3)
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t, NULL */
#include <stdlib.h>        /* for malloc, realloc */
#include <stdint.h>        /* for uint64_t */
#include <stdbool.h>       /* for bool */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t) */

/* Static cache for already computed terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* capacity of the array */
static idx_t last_checked = 4;   /* last integer examined; next is 5 */

/* Grow the cache to accommodate at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = new_cap > 8 ? new_cap : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Should never happen in the test harness. */
        abort();
    }
    cache = tmp;
    cache_cap = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, but harmless */

    /* Extend cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        last_checked++;    /* move to next integer */

        if (last_checked < 5)          /* skip 1,2,3,4 – we start at 5 */
            continue;

        if (!is_prime_small((uint64_t)last_checked))
            continue;                 /* not prime */

        /* Check for twin prime partner (>3). */
        bool has_twin = false;
        if (last_checked > 5 &&
            is_prime_small((uint64_t)(last_checked - 2)))
            has_twin = true;
        if (!has_twin && last_checked > 3 &&
            is_prime_small((uint64_t)(last_checked + 2)))
            has_twin = true;

        if (!has_twin)
            continue;                 /* not a twin prime term */

        /* Store the prime.  The cast to seq_t is safe when the value
           fits; overflow is acceptable because the sequence is checked
           only for terms that fit into seq_t. */
        grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];              /* a is 1‑indexed */
}
