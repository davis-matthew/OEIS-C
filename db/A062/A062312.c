/*
 * A062312  Positive nonprime numbers squared.
 * https://oeis.org/A062312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n‑th positive nonprime integer)² (OEIS A062312 – square
 *            of the n-th positive nonprime integer)
 * Range:     u8 n <= 9, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Lazy cache of already computed squares. */
    static seq_t *cache      = NULL;   /* computed values              */
    static size_t cache_cap = 0;       /* capacity of the cache        */
    static size_t cache_len = 0;       /* number of stored values      */
    static idx_t next_candidate = 1;   /* next integer to test for primality */

    /* The sequence is defined for n >= 1.  For safety return 0 when n==0. */
    if (n == 0) return 0;

    /* Insert terms until the cache holds at least n values. */
    while (cache_len < (size_t)n)
    {
        /* Skip primes to find the next non‑prime integer. */
        while (is_prime(next_candidate)) ++next_candidate;

        /* Compute its square in the appropriate integer type. */
        seq_t val = (seq_t)next_candidate * (seq_t)next_candidate;

        /* Grow the cache if it is full. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;   /* store the new term */
        ++next_candidate;           /* prepare next search */
    }

    return cache[n - 1];  /* sequences are 1‑based */
}
