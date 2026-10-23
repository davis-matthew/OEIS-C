/*
 * A105710  Numbers k such that 9973*k + 1009 is prime.
 * https://oeis.org/A105710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th integer k (k ≥ 0) such that 9973*k + 1009 is prime.
   The sequence is defined for n ≥ 1 (a(1)=0).  The implementation caches results
   so that a call with any n uses only the necessary search up to that n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"   /* bool is_prime_uint64(uint64_t n); */

/* seq_t and idx_t are defined by the build script.
   No redeclaration of seq_t is performed here. */

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 (value does not matter). */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found k's. */
    static uint64_t *cache = NULL;
    static idx_t cache_size      = 0;   /* number of entries cached   */
    static idx_t cache_capacity  = 0;   /* number of slots allocated */
    static uint64_t next_k = 0;         /* next k to test (starting at 0) */

    /* Extend cache until it contains at least n entries. */
    while (cache_size < n) {
        uint64_t candidate = 9973ULL * next_k + 1009ULL;

        if (is_prime_uint64(candidate)) {
            /* Allocate more space if needed. */
            if (cache_size == cache_capacity) {
                idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = next_k;
        }
        /* Move on to the next candidate.  The sequence is infinite,
           so this loop will eventually terminate for any finite n. */
        next_k++;
    }

    /* Return the (n‑1)-th cached value, converted to seq_t. */
    return (seq_t)cache[n - 1];
}
