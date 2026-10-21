/*
 * A201860  Primes of the form 9n^2 - 2.
 * https://oeis.org/A201860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 9*n^2−2
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 2052, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches successive values of k (starting from 1)
   and collects the values 9*k^2−2 that are prime.  The list of primes
   is cached so that calls with arbitrary n are answered in
   logarithmic time after the cache has been built up to the requested
   size.  Primality testing uses the generic routine provided by the
   build environment. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"

static seq_t *prime_cache = NULL;   /* dynamic array of found primes   */
static size_t cache_len = 0;        /* number of stored primes         */
static size_t cache_cap = 0;        /* current capacity of prime_cache */
static idx_t    next_k   = 0;        /* next k to test (starts at 0)    */

/* Ensure prime_cache can hold at least 'need' elements.  Capacity
   is increased exponentially to keep amortised O(1) growth. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failed – terminate the program, as the sequence
           cannot be computed without memory.  In a real application
           you might want to handle this more gracefully. */
        abort();
    }
    prime_cache = new_arr;
    cache_cap = new_cap;
}

/* Compute and return the nth term of the sequence.
   n is 1-based: a(1) is the first prime of the form 9*k^2−2.   */
seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined behaviour for n=0; return 0 to avoid UB. */
        return (seq_t)0;

    /* Extend the cache until it contains at least n primes. */
    while (cache_len < n) {
        /* Test the next k value. */
        ++next_k;
        seq_t k = (seq_t)next_k;
        seq_t val = ((seq_t)9 * k * k) - (seq_t)2;  /* 9*k^2−2 */

        /* Check primality. */
        if (isPrimeGeneric(val)) {
            ensure_cache_capacity(cache_len + 1);
            prime_cache[cache_len++] = val;
        }
    }

    return prime_cache[n - 1];
}
