/*
 * A142422  Primes congruent to 10 mod 49.
 * https://oeis.org/A142422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 126, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142422: primes congruent to 10 (mod 49) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

#define MAX_CACHE 200000   /* should cover all tests (n up to about 1e5) */

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t cache[MAX_CACHE];
    static size_t cache_size = 0;
    static idx_t last_k = 0;                /* k of the last tested candidate */

    /* guard against n < 1 (offset starts at 1) */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* extend the cache until it contains at least n primes */
    while (cache_size < (size_t)n) {
        ++last_k;
        /* construct candidate 10 + 49·k */
        uint64_t cand64 = 10ULL + 49ULL * (uint64_t)last_k;
        seq_t cand = (seq_t)cand64;

        /* test for primality */
        if (is_prime_big(cand)) {
            if (cache_size < MAX_CACHE) {
                cache[cache_size++] = cand;
            } else {
                /* cache overflow – should not happen for test limits */
                abort();
            }
        }
    }

    return cache[n - 1];     /* zero‑based indexing in the cache */
}
