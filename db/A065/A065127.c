/*
 * A065127  Nonsquares with number of prime factors equal to twice the number
 *          of distinct prime factors.
 * https://oeis.org/A065127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A065127:
   nonsquare integers n such that sum of prime exponents = 2 × ω(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static seq_t *cache   = NULL;
    static idx_t  cached_len = 0;
    static idx_t  cached_cap = 0;
    static uint64_t candidate = 2;   /* next integer to test */

    if (n == 0)   /* offset is 1 */
        return 0;

    while (cached_len < n) {
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt = 0;

        factor_u64(candidate, primes, exps, &cnt);
        if (cnt > 0) {
            uint64_t sum_exps = 0;
            bool all_even = true;
            for (size_t i = 0; i < cnt; ++i) {
                sum_exps += exps[i];
                if (exps[i] & 1ULL)
                    all_even = false;
            }
            if (sum_exps == 2ULL * cnt && !all_even) {
                /* enlarge cache if necessary */
                if (cached_len >= cached_cap) {
                    cached_cap = cached_cap ? cached_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)malloc(cached_cap * sizeof(seq_t));
                    if (!tmp) abort();              /* unrecoverable */
                    if (cache) {
                        memcpy(tmp, cache, cached_len * sizeof(seq_t));
                        free(cache);
                    }
                    cache = tmp;
                }
                cache[cached_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }

    return cache[n - 1];
}
