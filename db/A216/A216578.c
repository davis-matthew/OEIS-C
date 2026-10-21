/*
 * A216578  Prime numbers which cannot be written in the form x^2 + 10*y^2.
 * https://oeis.org/A216578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes that cannot be written as x^2 + 10*y^2
   (equivalently primes not congruent to 1, 9, 11, or 19 modulo 40). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/ensure_capacity.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t candidate = 2;   /* start from the first prime */

    if (n == 0) return 0;  /* offset not used but safe */

    while (cache_len < (size_t)n) {
        if (is_prime(candidate)) {
            uint64_t rem = candidate % 40ULL;
            if (rem != 1ULL && rem != 9ULL && rem != 11ULL && rem != 19ULL) {
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
                cache[cache_len] = (seq_t)candidate;
                cache_len++;
            }
        }
        candidate++;
    }
    return cache[n - 1];
}
