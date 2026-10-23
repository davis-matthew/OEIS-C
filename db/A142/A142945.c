/*
 * A142945  Primes congruent to 45 mod 64.
 * https://oeis.org/A142945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142945: primes congruent to 45 mod 64 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t last_k      = 0;   /* last k such that 64*k+45 was prime */

    /* ensure the cache holds at least n elements */
    if ((size_t)n > cache_size) {
        /* grow the cache capacity if needed */
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find more primes congruent to 45 mod 64 */
        for (idx_t k = last_k + 1; cache_size < (size_t)n; ++k) {
            idx_t cand = 64ULL * k + 45ULL;   /* candidate number */
            if (is_prime(cand)) {
                cache[cache_size++] = (seq_t)cand;
                last_k = k;
            }
        }
    }

    return cache[n - 1];
}
