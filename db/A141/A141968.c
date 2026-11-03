/*
 * A141968  Primes congruent to 9 mod 28.
 * https://oeis.org/A141968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 545, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 900 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed: nth prime congruent to 9 mod 28, via incremental search. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static state: cache of found primes, current candidate */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 9;          /* first integer ≡ 9 (mod 28) */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* grow cache until it contains at least n primes */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;  /* double capacity */
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 28; /* maintain the residue */
    }

    return cache[n - 1];
}
