/*
 * A111168  Semiprimes n such that 2*n - 1 is also a semiprime.
 * https://oeis.org/A111168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 5264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_seq_array, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A111168: Semiprimes n such that 2*n-1 is also a semiprime.
 * Computed by iterating over natural numbers, testing the semiprimality
 * of n and of 2*n-1 using the provided helper `is_semiprime_uint64`.
 * Results are cached so that multiple calls in any order are inexpensive.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/ensure_seq_array.h"

seq_t a(idx_t n)
{
    /* 1st term corresponds to a(1) = 25 */
    if (n < 1) return (seq_t)0;   /* undefined for n==0, but harmless */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array   */
    static idx_t cache_size = 0;   /* number of terms currently stored */

    /* static variable to remember where to resume the search */
    static uint64_t next_candidate = 4;   /* smallest semiprime candidate */

    /* Expand cache until it contains at least n terms */
    while (cache_size < n) {
        /* Find next n such that both n and 2*n-1 are semiprime */
        for ( ; ; next_candidate++) {
            if (is_semiprime_uint64(next_candidate)) {
                /* 2*n-1 fits in uint64_t for the tested range */
                uint64_t t = next_candidate * 2U - 1U;
                if (is_semiprime_uint64(t))
                    break;   /* success – current next_candidate is a valid term */
            }
        }

        /* Ensure sufficient capacity */
        if (cache_size == (idx_t)cache_cap)
            ensure_seq_array(&cache, &cache_cap, cache_size + 1);

        /* Store the term in the cache */
        cache[cache_size++] = (seq_t)next_candidate;

        /* Advance candidate for subsequent searches */
        next_candidate++;   /* skip this term */
    }

    /* Return the nth term (1-based indexing) */
    return cache[n - 1];
}
