/*
 * A080191  Primes p such that p is the largest of all prime factors of the
 *          numbers between the prime preceding 2*p and the next prime.
 * https://oeis.org/A080191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 5128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080191: primes p such that p is the largest prime factor of all numbers
   in the interval [2p, nextprime(2p)‑1] */

/* Utility used by the generated code:
   uint64_t next_prime(uint64_t start);  // returns the smallest prime > start
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;         /* allocated capacity of cache   */
static size_t cache_len = 0;         /* number of values stored       */
static uint64_t prime_to_test = 2;   /* next prime to examine        */

/* Grow cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap * 2U : 4U;
    if (cap < new_cap)
        cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) abort();               /* out‑of‑memory */
    cache = tmp;
    cache_cap = cap;
}

/* Return the n‑th term (n ≥ 1) of A080191 */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, return 0 as a safe guard */
        return (seq_t)0;

    /* if already precomputed, return immediately */
    if ((size_t)(n - 1) < cache_len)
        return cache[(size_t)(n - 1)];

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        uint64_t p = prime_to_test;                 /* current prime   */
        uint64_t two_p = p << 1;                    /* 2 * p           */
        uint64_t q = next_prime(two_p);             /* next prime after 2p */
        uint64_t t = (q - 1) >> 1;                  /* (q-1)/2         */
        uint64_t r1 = next_prime(p);                /* next prime after p */

        if (r1 > t) {                               /* membership test */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }

        prime_to_test = next_prime(p);              /* advance to next prime */
    }

    return cache[(size_t)(n - 1)];
}
