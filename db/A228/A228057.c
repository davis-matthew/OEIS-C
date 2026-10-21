/*
 * A228057  Odd numbers of the form p * m^2, where p is prime and m > 1.
 * https://oeis.org/A228057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228057: odd numbers of the form p * m^2 (p prime, m > 1), 
 * sorted ascending.  The function builds the sequence on demand,
 * storing already computed terms in a static cache.  For each odd
 * integer, we factor it using factor_u64 and keep those with exactly
 * one prime factor of odd exponent (excluding primes themselves). */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next odd integer to test */
    static uint64_t next_candidate = 3;

    /* Ensure we have at least n terms cached */
    while (cache_len < n) {
        /* Skip even numbers and increase by 2 */
        if ((next_candidate & 1) == 0) next_candidate++;
        while (1) {
            /* Test if next_candidate belongs to the sequence */
            if (next_candidate > 1) {
                /* Factor using available helper */
                uint64_t primes[64], exps[64];
                size_t cnt = 0;
                factor_u64(next_candidate, primes, exps, &cnt);

                /* Count prime factors with odd exponent */
                size_t oddCount = 0;
                for (size_t i = 0; i < cnt; ++i)
                    if (exps[i] & 1UL) oddCount++;

                /* Accept if exactly one odd exponent and not a prime itself */
                if (oddCount == 1 && !(cnt == 1 && exps[0] == 1)) {
                    /* Store value in the cache */
                    append_candidate(&cache, &cache_len, &cache_cap,
                                     (seq_t)next_candidate);
                    break;
                }
            }
            next_candidate += 2;
        }
        /* Advance candidate for next search */
        next_candidate += 2;
    }
    return cache[n - 1];
}
