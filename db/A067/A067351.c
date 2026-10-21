/*
 * A067351  Numbers k such that sigma(k) + phi(k) has exactly 2 distinct prime
 *          divisors.
 * https://oeis.org/A067351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that sigma(k) + phi(k) has exactly two distinct
 *            prime divisors
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, count_distinct_primes, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */
#include "../utils/calc_phi.h"          /* seq_t calc_phi(seq_t m); */
#include "../utils/count_distinct_primes.h" /* int count_distinct_primes(uint64_t n); */
#include "../utils/seq_realloc.h"       /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

static seq_t *cache = NULL;   /* Array of accepted k values */
static size_t cache_len = 0;  /* Number of elements stored */
static size_t cache_cap = 0;  /* Capacity of the array */
static idx_t  last_k   = 0;   /* Last k that was examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort();
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Main sequence function.  Returns the n‑th member of A067351.
   n is 1‑based; values for n == 0 are undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset 1 – caller should never request 0 */

    /* Build the cache until we have at least n elements. */
    while (cache_len < n)
    {
        /* Next candidate k */
        last_k++;

        /* Compute sigma(k) + phi(k). */
        seq_t sigma = sigma_of(last_k);
        seq_t phi   = calc_phi((seq_t)last_k);
        seq_t sum   = sigma + phi;

        /* Convert the sum to uint64_t for prime‑factor counting. */
        uint64_t sum64 = (uint64_t)sum;

        /* Count distinct prime factors of the sum. */
        if (count_distinct_primes(sum64) == 2)
        {
            /* Store this k into the cache. */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    /* Cache is 0‑based; convert to 1‑based index. */
    return cache[n - 1];
}
