/*
 * A132237  Primes congruent to {7, 23} mod 30.
 * https://oeis.org/A132237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 989 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Uses incremental search over numbers ≡7 or 23 (mod 30) and
   caches results for repeated calls.  */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t k = 0;
    static bool next_is_7 = true;

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    while (n > cache_len) {
        seq_t cand;
        if (next_is_7) {
            cand = (seq_t)(30ULL * (unsigned long long)k + 7ULL);
        } else {
            cand = (seq_t)(30ULL * (unsigned long long)k + 23ULL);
        }

        if (is_prime((idx_t)cand)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cand;
        }

        if (next_is_7)
            next_is_7 = false;
        else {
            next_is_7 = true;
            ++k;
        }
    }

    return cache[n - 1];
}
