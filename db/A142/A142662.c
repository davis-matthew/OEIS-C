/*
 * A142662  Primes congruent to 51 mod 56.
 * https://oeis.org/A142662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142662: Primes congruent to 51 (mod 56) – a(n) is the n‑th such prime (n ≥ 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0 so callers can test the offset. */
    if (n == 0)
        return 0;

    /* Cached list of primes we have already found. */
    static seq_t *cache   = NULL;   /* array holding the primes */
    static size_t cache_cnt = 0;    /* number of elements stored */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Next candidate number ≡ 51 (mod 56), starting with the first known prime. */
    static uint64_t next_candidate = 107;   /* 56 * 1 + 51 */

    /* Extend the cache until it contains at least n primes. */
    while (cache_cnt < (size_t)n) {
        if (is_prime(next_candidate)) {
            /* Grow storage if necessary. */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                     /* allocation failed */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_cnt = 0;
                    return 0;                   /* sentinel value */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime found. */
            cache[cache_cnt++] = (seq_t)next_candidate;
        }
        /* Advance to the next candidate in the arithmetic progression. */
        next_candidate += 56;
    }

    /* Return the (n‑1)-th element of the cache (0‑based indexing). */
    return cache[n - 1];
}
