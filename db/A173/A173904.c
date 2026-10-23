/*
 * A173904  Numbers k such that 7*k+-1 are prime.
 * https://oeis.org/A173904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer k (n ≥ 1) such that 7·k − 1 and 7·k + 1
 *            are both prime
 * Range:     u8 n <= 1, u16 n <= 137, u32 n <= 164, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of found k-values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* last k tested (undefined on first call) */
    static uint64_t last_k = 0;

    if (n == 0) {
        /* offset is 1; behaviour for n = 0 is irrelevant */
        return 0;
    }

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Find next admissible k */
        while (true) {
            ++last_k;
            /* 7·k as a seq_t value */
            seq_t base = (seq_t)(7ULL * last_k);
            seq_t p1 = base - 1;  /* 7·k − 1 */
            seq_t p2 = base + 1;  /* 7·k + 1 */
            if (is_prime_big(p1) && is_prime_big(p2)) {
                break;
            }
        }

        /* Ensure capacity for the new element */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)last_k;
    }

    return cache[(size_t)n - 1];
}
