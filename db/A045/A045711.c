/*
 * A045711  Primes with first digit 5.
 * https://oeis.org/A045711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1055, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   first_digit_is_five, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: Primes whose first decimal digit is 5
 *  The function iteratively tests integers for primality
 *  and stores those with leading digit 5.  The cache is
 *  expanded as needed so that a(n) can be returned for any n >= 1
 *  in O(n) time with a small memory footprint.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n) */
#include "../utils/first_digit_is_five.h"

/*  External linkage required by the build.  */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;        /* array of cached primes   */
    static size_t cache_len = 0;         /* number of terms cached   */
    static size_t cache_cap = 0;         /* allocated capacity       */
    static uint64_t next_candidate = 5u; /* next integer to test     */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Extend storage if necessary. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {  /* allocation failed – abort safely */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search until we find the next prime starting with 5. */
        while (true)
        {
            if (first_digit_is_five(next_candidate) &&
                is_prime_small((seq_t)next_candidate))
            {
                cache[cache_len++] = (seq_t)next_candidate;
                /* Move to the next integer for further searching. */
                next_candidate++;
                break;
            }
            /* Skip composites and numbers not starting with 5. */
            next_candidate++;
        }
    }

    /* Cache is 0‑based, but the sequence is 1‑based. */
    return cache[n - 1];
}
