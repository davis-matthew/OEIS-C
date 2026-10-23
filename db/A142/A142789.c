/*
 * A142789  Primes congruent to 19 mod 60.
 * https://oeis.org/A142789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 19 modulo 60
 * Range:     u8 n <= 4, u16 n <= 380, u32 n <= 381, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored sequence terms          */
    static size_t cache_len = 0;     /* number of terms currently stored */
    static size_t cache_cap = 0;     /* allocated capacity               */
    static idx_t next_candidate = 19;/* next candidate of form 60k+19   */

    if (n == 0) return (seq_t)0;                /* unused – offset is 1          */

    if (cache_len >= n)                       /* already computed               */
        return cache[n - 1];

    /* extend cache until it holds at least n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;

        /* test the candidate for primality */
        if (is_prime_big(cand)) {
            /* ensure enough space in cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof (seq_t));
                if (!tmp) abort();            /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;        /* store the prime          */
        }

        /* advance to the next number of the form 60k+19 */
        next_candidate += 60;
    }

    return cache[n - 1];
}
