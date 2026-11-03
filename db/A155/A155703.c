/*
 * A155703  Primes p such that 2*p^2 + 16*p + 23 is also prime.
 * https://oeis.org/A155703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 491, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 955 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime p such that 2p^2 + 16p + 23 is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t cand = 2; /* next integer to test */

    while (cache_size < n) {
        if (is_prime(cand)) {
            seq_t val = 2 * (seq_t)cand * cand + 16 * (seq_t)cand + 23;
            if (is_prime((idx_t)val)) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = (seq_t)cand;
            }
        }
        ++cand;
    }

    return cache[n - 1];
}
