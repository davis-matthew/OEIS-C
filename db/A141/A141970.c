/*
 * A141970  Primes congruent to 13 mod 28.
 * https://oeis.org/A141970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 540, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime congruent to 13 modulo 28 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t next_candidate = 13;  /* first number ≡13 (mod 28) */

    if (n == 0)          /* undefined for index 0; return 0 for safety */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_cnt < n) {
        /* Expand the cache if necessary. */
        if (cache_cnt == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap + cache_cap / 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* termination on allocation failure */
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Test the current candidate for primality. */
        if (is_prime((idx_t)next_candidate)) {
            cache[cache_cnt++] = (seq_t)next_candidate;
        }
        /* Advance to the next number in the congruence class. */
        next_candidate += 28ULL;
    }

    return cache[n - 1];
}
