/*
 * A088173  Primes whose successive differences are increasing squares.
 * https://oeis.org/A088173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Primes whose successive differences are increasing squares. The
 *            first term is 2. For every subsequent term we add the smallest
 *            square greater than the previous difference that produces a
 *            prime. The function builds the sequence lazily and memoises
 *            previously found terms so that subsequent calls, e...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 13, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; we simply ignore n==0. */
    if (n == 0) return 0;

    /* Lazy, thread‑unsafe static cache. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    if (cache_cap == 0) {
        /* Initial capacity – grow as needed. */
        cache_cap = 32;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;
        /* First term */
        cache[0] = (seq_t)2;
        cache_size = 1;
    }
    /* Grow cache if necessary. */
    while (cache_size < (size_t)n) {
        /* Determine previous term and its difference from the one before it. */
        seq_t prev = cache[cache_size - 1];
        seq_t prev_diff = 0;
        if (cache_size > 1) {
            prev_diff = cache[cache_size - 1] - cache[cache_size - 2];
        }

        /* Starting square is the smallest with s^2 > prev_diff. */
        idx_t s_start = (idx_t)(floor_sqrt(prev_diff)) + 1;

        /* Search for the next prime. */
        seq_t next = 0;
        for (idx_t s = s_start; ; ++s) {
            seq_t square = (seq_t)s * (seq_t)s;
            seq_t candidate = prev + square;
            if (is_prime_big(candidate)) {
                next = candidate;
                break;
            }
        }

        /* Append to cache. */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) { free(cache); return 0; }
            cache = tmp;
        }
        cache[cache_size++] = next;
    }

    return cache[n - 1];
}
