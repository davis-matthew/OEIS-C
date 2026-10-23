/*
 * A023215  Primes p such that 4*p + 7 is also prime.
 * https://oeis.org/A023215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that 4*p + 7 is also prime (OEIS
 *            A023215)
 * Range:     u8 n <= 6, u16 n <= 317, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static uint64_t last_cand = 3;  /* odd candidate to test */

static void grow_cache(void) {
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* on allocation failure leave unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, caller responsibility */

    /* Grow cache until it contains at least n terms */
    while (cache_sz < (size_t)n) {
        seq_t p = (seq_t)last_cand;
        if (is_prime_small(p)) {
            seq_t q = 4 * p + 7;
            if (is_prime_small(q)) {
                if (cache_sz >= cache_cap) grow_cache();
                cache[cache_sz++] = p;
            }
        }
        /* advance to next odd integer */
        last_cand += 2;
    }
    return cache[n - 1];
}
