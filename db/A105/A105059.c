/*
 * A105059  Numbers n such that 100000n - 1 is prime.
 * https://oeis.org/A105059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m such that 100000*m - 1 is prime
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"   /* is_prime_uint64(uint64_t) */

/* Cached list of found terms. */
static seq_t *cache   = NULL;      /* array of found n's */
static idx_t cache_size = 0;       /* number of elements currently stored */
static idx_t cache_cap   = 0;       /* allocated capacity of the array */
static idx_t last_checked = 0;     /* last n that was examined */

static void grow_cache(void)
{
    idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

static void add_cache(seq_t val)
{
    if (cache_size == cache_cap)
        grow_cache();
    cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    /* Ensure we have computed the first n terms. */
    while (cache_size < n)
    {
        last_checked++;
        /* Compute 100000*last_checked - 1 safely as uint64_t. */
        uint64_t m    = (uint64_t)last_checked;
        uint64_t value = 100000ULL * m - 1ULL;   /* value to test for primality */
        if (is_prime_uint64(value))
            add_cache((seq_t)last_checked);
    }
    return cache[n - 1];
}
