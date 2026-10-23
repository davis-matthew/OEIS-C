/*
 * A171518  Primes p such that 3*p-+8 are primes.
 * https://oeis.org/A171518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 315, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p for which both 3*p-8
   and 3*p+8 are also primes.  It builds the sequence on demand
   and memoises the results.  The implementation works for any
   seq_t as long as the required utility functions are available. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;          /* cached primes */
static idx_t cached_count = 0;       /* number of stored terms */
static size_t cache_cap = 0;         /* current capacity of cache   */
static uint64_t last_checked = 1;    /* last integer examined      */

/* Ensure the cache can hold at least 'need' elements.  */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)need) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n>=1 */

    /* Extend the cache until it contains at least n terms. */
    while (cached_count < n) {
        /* Find the next prime candidate.  */
        uint64_t p = last_checked + 1;
        while (!is_prime(p)) ++p;
        last_checked = p;

        /* Check the property 3*p-8 and 3*p+8 are primes.  */
        seq_t sp = (seq_t)p;
        seq_t left  = 3 * sp - (seq_t)8;
        seq_t right = 3 * sp + (seq_t)8;
        if (isPrimeGeneric(left) && isPrimeGeneric(right)) {
            /* Store the qualifying prime. */
            ensure_cache_capacity(cached_count + 1);
            cache[cached_count++] = sp;
        }
    }
    return cache[n - 1];
}
