/*
 * A168079  Numbers n such that exactly one of n+-1, n+-2 and n+-3 is prime.
 * https://oeis.org/A168079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 53, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS sequence A168079
 *  Numbers n such that exactly one of n±1, n±2, n±3 is prime
 *  (offset 1 – a(1) = -1)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

/* Test if a 64‑bit value is prime – only positive integers are prime. */
static inline bool is_prime_u64(uint64_t v)
{
    if (v < 2u) return false;
    return is_prime((idx_t)v);
}

/* Does the integer x satisfy the definition?  x ≥ 1 is assumed. */
static inline bool qualifies(uint64_t x)
{
    unsigned primes = 0;   /* count of primes among the six neighbors */

    /* ±1 */
    if (is_prime_u64(x + 1u)) ++primes;
    if (x > 1u && is_prime_u64(x - 1u)) ++primes;

    /* ±2 */
    if (is_prime_u64(x + 2u)) ++primes;
    if (x > 2u && is_prime_u64(x - 2u)) ++primes;

    /* ±3 */
    if (is_prime_u64(x + 3u)) ++primes;
    if (x > 3u && is_prime_u64(x - 3u)) ++primes;

    return primes == 1u;
}

/* Cached positive terms of the sequence.  a(1) = -1 is handled
 * specially without storing it in the cache. */
static uint64_t *pos_cache      = NULL;  /* array of uint64_t values */
static size_t    pos_cache_cap  = 0;
static size_t    pos_cache_len  = 0;
static uint64_t  next_candidate = 1u;    /* next integer to test */

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    /* a(1) = -1 */
    if (n == 1u) return (seq_t)-1;

    /* Ensure the cache is initialized. */
    if (pos_cache_len == 0u) {
        pos_cache_cap = 16u;
        pos_cache     = (uint64_t *)malloc(pos_cache_cap * sizeof(uint64_t));
        /* next_candidate already set to 1 */
    }

    /* Generate terms until we have at least n-1 positive terms. */
    while (pos_cache_len < (size_t)(n - 1u)) {
        if (qualifies(next_candidate)) {
            /* Grow the cache if necessary. */
            if (pos_cache_len + 1u > pos_cache_cap) {
                size_t new_cap = pos_cache_cap * 2u + 1u;
                uint64_t *tmp = (uint64_t *)realloc(pos_cache, new_cap * sizeof(uint64_t));
                if (tmp) {
                    pos_cache    = tmp;
                    pos_cache_cap = new_cap;
                } else { /* allocation failed – abort */
                    /* In the rare event of OOM we return zero */
                    return (seq_t)0;
                }
            }
            pos_cache[pos_cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    /* Convert the cached uint64_t value to seq_t for the return. */
    return (seq_t)pos_cache[(size_t)(n - 2u)];
}
