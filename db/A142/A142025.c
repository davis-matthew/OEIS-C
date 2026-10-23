/*
 * A142025  Primes congruent to 21 mod 31.
 * https://oeis.org/A142025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 218, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142025: primes p such that p ≡ 21 (mod 31) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* static cache of discovered primes */
    static seq_t *prime_cache = NULL;
    static size_t       cache_cap = 0;
    static idx_t        cached_cnt = 0;
    static seq_t        last_candidate = 0;   /* last tested candidate */

    if (n == 0) return 0;   /* off‑by‑offset handling is irrelevant */

    /* grow cache if necessary */
    if (cache_cap < n) {
        size_t new_cap = n * 2;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (prime_cache) {
            for (size_t i = 0; i < cached_cnt; ++i)
                tmp[i] = prime_cache[i];
        }
        prime_cache = tmp;
        cache_cap = new_cap;
    }

    /* generate missing primes */
    while (cached_cnt < n) {
        if (cached_cnt == 0) {
            last_candidate = 83;
        } else {
            last_candidate += 31;
        }

        if (is_prime_small((uint64_t)last_candidate)) {
            prime_cache[cached_cnt] = last_candidate;
            ++cached_cnt;
        }
    }

    return prime_cache[n - 1];
}
