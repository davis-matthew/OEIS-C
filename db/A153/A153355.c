/*
 * A153355  Numbers k such that 5k-1 is a prime.
 * https://oeis.org/A153355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers k for which 5k-1 is prime
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static seq_t next_k       = 4;   /* start with the first eligible k */

/* enlarge the cache when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* generate terms until we have at least n of them */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        if (cache_size == cache_cap)
            grow_cache();

        seq_t candidate = 5 * next_k - 1;
        if (is_prime_big(candidate))
            cache[cache_size++] = next_k;

        next_k++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* out of range – undefined behaviour normally */
    ensure_cache(n);
    return cache[n - 1];
}
