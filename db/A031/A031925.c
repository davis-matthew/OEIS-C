/*
 * A031925  Upper prime of a difference of 6 between consecutive primes.
 * https://oeis.org/A031925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code searches for gaps of 6 between consecutive primes
   and returns the upper prime of such a gap. */
#include "../utils/is_prime.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* statically remembered state of the prime search */
    static idx_t last_prime = 2;      /* last prime found   */
    static idx_t candidate  = 3;      /* next candidate to test */

    if (n == 0) return 0;   /* offset is 1; the function is not used for n<1 */

    /* extend the cache until it contains the requested term */
    while (cache_len < n) {
        /* advance candidate to the next prime */
        while (!is_prime(candidate)) ++candidate;

        /* if the gap is exactly 6, store the upper prime */
        if (candidate - last_prime == 6) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = alloc_seq_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
                free_seq_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        /* move forward */
        last_prime = candidate;
        ++candidate;
    }

    return cache[n - 1];
}
