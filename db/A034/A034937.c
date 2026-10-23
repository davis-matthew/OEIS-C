/*
 * A034937  Primes p of the form 6k-1 such that 2*p + 3 is prime.
 * https://oeis.org/A034937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034937
 * Primes p ≡ 5 (mod 6) such that 2p + 3 is also prime.
 * Computed by an incremental search over candidates of the form 6k-1. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */
#include "../utils/is_prime_small.h"/* provides bool is_prime_small(seq_t) */
#include "../utils/seq_alloc_new_array.h" /* provides seq_t* seq_alloc_new_array(size_t) */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;              /* k for candidate p = 6k-1 */

    /* grow the cache as needed */
    if (cache_len < n) {
        if (cache_cap < cache_len + 128) { /* grow in chunks of 128 */
            size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            if (cache) free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* search until we have n terms */
        while (cache_len < n) {
            seq_t p = (seq_t)6 * (seq_t)next_k - 1;  /* candidate p ≡ 5 (mod 6) */
            if (is_prime_small(p)) {
                seq_t q = 2 * p + 3;
                if (is_prime_small(q)) {
                    cache[cache_len++] = p;
                }
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
