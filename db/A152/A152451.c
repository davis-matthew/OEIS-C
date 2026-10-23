/*
 * A152451  From every interval (2^(m-1), 2^m), we remove primes p for which
 *          2^m-p is a prime; the sequence gives the remaining odd primes.
 * https://oeis.org/A152451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_log2, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A152451: the odd primes p such that
   in the unique interval (2^(m‑1), 2^m) containing p,
   the number 2^m – p is NOT prime.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/floor_log2.h" /* static idx_t floor_log2(idx_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored so far */
    static idx_t cache_cap = 0;   /* allocated capacity of cache     */
    static idx_t next_candidate = 3;/* odd number to examine next      */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        /* Find the next odd prime. */
        idx_t p = next_candidate;
        while (!is_prime(p))
            p += 2;                /* skip evens */

        /* Determine the power of two defining p's interval. */
        unsigned m = (unsigned)floor_log2(p) + 1; /* m ≥ 1 */
        uint64_t pow2m = ((uint64_t)1) << m;
        uint64_t q = pow2m - (uint64_t)p;        /* 2^m – p */

        /* Keep p if its partner q is not prime. */
        if (!is_prime((idx_t)q))
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }

        /* Advance to the next odd number to test. */
        next_candidate = p + 2;
    }

    /* Return the requested term (1‑based indexing). */
    return cache[n - 1];
}
