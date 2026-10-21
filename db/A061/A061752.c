/*
 * A061752  Positive integers k such that k! is divisible by (k+1)^4.
 * https://oeis.org/A061752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   satisfies_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A061752 – Positive integers k such that k! is divisible by (k+1)^4
 *
 *  Algorithm:
 *  For each candidate k we factor m = k+1.  For every prime factor p^e of m
 *  we compute the exponent of p in k! by Legendre's formula:
 *      v_p(k!) = floor(k/p) + floor(k/p^2) + ...
 *  The condition is v_p(k!) ≥ 4·e for all prime factors of m.
 *  We generate the sequence lazily, caching all found terms so that later
 *  calls can use them directly.  Factorisation is performed by simple
 *  trial division which is efficient enough for the required range
 *  (k ≲ 10^6 for the first ~10 000 terms).
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/satisfies_k.h"

/* ------------------------------ main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    /* First call initialises static storage. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_k = 0;     /* last found term */

    if (n == 0) return (seq_t)0;  /* OEIS offset is 1 – behaviour for n < 1 irrelevant */

    /* Grow cache until it holds the requested term. */
    while (cache_len < (size_t)n) {
        /* Find next k that satisfies the condition. */
        uint64_t k = next_k + 1;
        while (!satisfies_k(k)) ++k;
        next_k = k;

        /* Ensure cache has enough space. */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 8;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)k;
    }

    return cache[n - 1];
}
