/*
 * A154610  Numbers n such that 13n + 5 is prime.
 * https://oeis.org/A154610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 457, u32 n <= 544, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 13*n + 5 is prime (OEIS A154610) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Treat index 0 as out of range (not used by the OEIS interface). */
    if (n == 0)
        return 0;

    /* Static cache of already generated terms. */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_len  = 0;      /* how many terms are stored */
    static size_t cache_cap  = 0;      /* current capacity of the array */
    static idx_t  next_candidate = 0;  /* next n to test */

    /* Return the value if we already have it. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Grow the cache until it contains the requested term. */
    while (cache_len < n) {
        /* Search for the next n such that 13*n+5 is prime. */
        while (true) {
            seq_t val = (seq_t)13 * (seq_t)next_candidate + (seq_t)5;
            if (is_prime_big(val))
                break;          /* found a valid n */
            next_candidate++;   /* try the next integer */
        }

        /* Ensure we have space for a new term. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)        /* memory allocation failed */
                abort();
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Store the discovered term. */
        cache[cache_len] = (seq_t)next_candidate;
        cache_len++;

        /* Advance to the next integer for future searches. */
        next_candidate++;
    }

    /* Now cache_len >= n, return the requested element. */
    return cache[n - 1];
}
