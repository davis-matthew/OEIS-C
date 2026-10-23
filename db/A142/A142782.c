/*
 * A142782  Primes congruent to 55 mod 59.
 * https://oeis.org/A142782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 55 modulo 59
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"    /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* Static cache of primes found so far. */
    static seq_t *cache  = NULL;
    static size_t cache_cnt = 0, cache_cap = 0;
    static idx_t next_k = 2;      /* k such that the candidate is 59*k + 55 */

    /* If we haven't found n primes yet, grow the cache and generate more. */
    if ((size_t)n > cache_cnt) {
        /* Ensure the cache has room for n elements. */
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure -> 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate primes until we have at least n of them. */
        while (cache_cnt < (size_t)n) {
            /* Compute the candidate using idx_t arithmetic (no overflow). */
            idx_t cand_i = 59 * next_k + 55;
            ++next_k;
            if (is_prime_big((seq_t)cand_i)) {
                cache[cache_cnt++] = (seq_t)cand_i;
            }
        }
    }
    return cache[(size_t)n - 1];
}
