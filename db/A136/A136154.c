/*
 * A136154  Composites one larger than a prime, with exactly five distinct
 *          prime factors.
 * https://oeis.org/A136154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite integer that is one larger than a prime
 *            and that itself has exactly five distinct prime factors
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/is_prime.h"

static seq_t *cache     = NULL;   /* memoised sequence values           */
static size_t cache_len = 0;      /* number of elements already stored  */
static size_t cache_cap = 0;      /* allocated capacity of the cache    */

/* Ensure the cache can store at least @need elements.              */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* caller should never use n=0 */

    /* Grow the cache until it contains the nth term.                   */
    while (cache_len < (size_t)n) {
        static idx_t cur = 2;              /* next integer to test */
        /* Search for the next prime.  The is_prime function is efficient
           for 64‑bit integers and is available in the utils library.       */
        while (!is_prime(cur)) ++cur;
        /* cur is prime; test the composite cur+1. */
        uint64_t candidate = (uint64_t)cur + 1;
        if (count_distinct_primes(candidate) == 5) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++cur;                            /* move to the next integer */
    }
    return cache[(size_t)n - 1];
}
