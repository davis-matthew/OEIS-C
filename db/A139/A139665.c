/*
 * A139665  Primes of the form x^2 + 840*y^2.
 * https://oeis.org/A139665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p = x^2 + 840*y^2, equivalently
 *            p ≡ 1,121,169,289,361,529 (mod 840)
 * Range:     u8 none, u16 n <= 175, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static inline bool good_mod(uint64_t v)
{
    uint64_t r = v % 840ULL;
    switch (r) {
        case 1ULL: case 121ULL: case 169ULL: case 289ULL:
        case 361ULL: case 529ULL: return true;
        default: return false;
    }
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 3ULL;   /* start at first odd >2 */

    while (cache_len < (size_t)n)
    {
        /* Skip candidates that are not in the congruence classes. */
        while (!good_mod(next_candidate))
            next_candidate += 2ULL;

        /* Test primality using the big‑integer routine. */
        if (is_prime_big((seq_t)next_candidate))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        next_candidate += 2ULL;   /* move to the next odd number */
    }

    return cache[n - 1];
}
