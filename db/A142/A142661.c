/*
 * A142661  Primes congruent to 47 mod 56.
 * https://oeis.org/A142661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 47 modulo 56.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index < 1 undefined by OEIS spec */

    /* static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 0;    /* k such that candidate is 56*k+47 */

    /* grow cache until we have at least n primes */
    while (cache_sz < (size_t)n) {
        uint64_t cand = 56ULL * next_k + 47ULL;   /* next candidate */
        if (is_prime(cand)) {
            /* expand array if needed */
            if (cache_sz >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);                 /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)cand;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
