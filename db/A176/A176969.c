/*
 * A176969  Numbers n such that n^2 + 13^2 is prime.
 * https://oeis.org/A176969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th even integer k such that k^2 + 13^2 is prime
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached_cap = 0;  /* capacity of cache */
    static idx_t cached_len = 0;  /* number of terms stored */
    static seq_t next_candidate = 2; /* next even n to test */

    if (n == 0) return 0; /* undefined for offset below 1 */

    /* Ensure the cache contains at least n terms */
    while (cached_len < n) {
        /* compute candidate^2 + 169 */
        seq_t val = next_candidate * next_candidate + (seq_t)169;
        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (cached_len == (idx_t)cached_cap) {
                size_t new_cap = cached_cap ? cached_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed, abort */
                    abort();
                }
                cache = tmp;
                cached_cap = (idx_t)new_cap;
            }
            cache[cached_len++] = next_candidate;
        }
        next_candidate += 2;  /* only even numbers can yield odd primes */
    }

    return cache[n - 1];
}
