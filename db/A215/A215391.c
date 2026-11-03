/*
 * A215391  Primes congruent to {1, 2} mod 13.
 * https://oeis.org/A215391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n where p_n is the n-th prime congruent to 1 or 2 (mod
 *            13)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 915 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* static cache for the primes of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime = 0;   /* last prime examined (inclusive) */

    /* first call: initialise with prime 2 if it qualifies */
    if (cache_len == 0) {
        if (2 % 13 == 1 || 2 % 13 == 2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)2);
        }
        last_prime = 2;
    }

    /* extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        last_prime = next_prime(last_prime);          /* next prime > last_prime */
        if (last_prime % 13 == 1 || last_prime % 13 == 2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)last_prime);
        }
    }

    return cache[n - 1];
}
