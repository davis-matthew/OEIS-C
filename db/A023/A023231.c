/*
 * A023231  Primes p such that 8*p + 7 is also prime.
 * https://oeis.org/A023231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 223, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p for which 8p+7 is also prime.
   It generates primes in increasing order, tests the condition, and
   memoises the qualifying terms.  The implementation works with any
   seq_t/idx_t defined by the build system. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of the sequence values */
static seq_t *term_cache = NULL;    /* array of found terms  */
static size_t  cache_len  = 0;     /* number of cached terms */
static size_t  cache_cap  = 0;     /* capacity of cache       */

/* Ensure the cache can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Append a new qualifying prime to the cache */
static void add_term(const seq_t *p)
{
    grow_cache(cache_len + 1);
    term_cache[cache_len++] = *p;
}

/* State used on successive calls */
static seq_t  next_candidate;   /* candidate primes to test    */
static bool    init_next_candidate = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    /* Initialise the candidate counter on first use */
    if (!init_next_candidate) {
        next_candidate = (seq_t)2;
        init_next_candidate = true;
    }

    /* Generate terms until we have at least 'n' of them */
    while (cache_len < (size_t)n) {
        /* Search for the next prime p */
        bool found = false;
        while (!found) {
            if (is_prime_big(next_candidate)) {
                /* Test 8*p+7 */
                seq_t test = next_candidate * (seq_t)8 + (seq_t)7;
                if (is_prime_big(test)) {
                    add_term(&next_candidate);
                    found = true;
                }
            }
            /* Next integer */
            next_candidate += (seq_t)1;
        }
    }

    return term_cache[(size_t)n - 1];
}
