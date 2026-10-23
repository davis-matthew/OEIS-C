/*
 * A119289  Prime numbers p such that there is no prime between 10*p and
 *          10*p+9 inclusive.
 * https://oeis.org/A119289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime p such that all numbers 10*p+d (d=0..9)
 *            are composite
 * Range:     u8 none, u16 n <= 624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */

    /* static state: cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;

    /* static state: last candidate checked */
    static uint64_t last_candidate = 1;

    /* grow cache if necessary */
    while (cache_cnt < (size_t)n) {
        /* find next prime */
        while (true) {
            last_candidate++;
            if (is_prime_small(last_candidate)) { /* prime found */
                bool ok = true;
                for (uint64_t d = 0; d < 10; ++d) {
                    uint64_t x = last_candidate * 10 + d;
                    if (is_prime_small(x)) {  /* found a prime in the range */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    /* store it */
                    if (cache_cnt >= cache_cap) {
                        ensure_capacity(cache_cnt + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                    }
                    cache[cache_cnt++] = (seq_t)last_candidate;
                }
                break; /* either stored or skipped */
            }
        }
    }

    return cache[n - 1];
}
