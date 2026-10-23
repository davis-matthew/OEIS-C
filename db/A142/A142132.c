/*
 * A142132  Primes congruent to 23 mod 37.
 * https://oeis.org/A142132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS ref: A142132 – Primes congruent to 23 modulo 37
 *
 * a(n) = the n‑th prime p such that p ≡ 23 (mod 37), for n ≥ 1.
 * The sequence starts:
 *   23, 97, 467, 541, 911, 1429, 1873, 2243, …
 *
 * Implementation detail:
 * The function keeps a static cache of already found primes.
 * When a request for a larger index arrives it simply continues
 * searching from the last found prime, checking each candidate
 * (enumerated as …, x, x+37, x+74, …) with the supplied
 * `is_prime` routine.  No external state is required other than
 * the cache.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache      = NULL;   /* array of primes found till now   */
    static size_t cache_len  = 0;      /* number of stored primes          */
    static size_t cache_cap  = 0;      /* size of allocated array          */

    if (n == 0) return 0;  /* offset is 1, a(0) is undefined */

    /* Ensure the array can hold `n` elements */
    if (n > cache_cap) {
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    /* Extend the cache up to the requested index */
    if (cache_len < (size_t)n) {
        /* Start from the first candidate after the last stored prime */
        seq_t cand;
        if (cache_len == 0) {
            cand = (seq_t)23;                /* first prime in the sequence   */
        } else {
            cand = cache[cache_len - 1] + (seq_t)37;
        }

        while (cache_len < (size_t)n) {
            if (is_prime((idx_t)cand)) {
                cache[cache_len++] = cand;
            }
            cand += (seq_t)37;                /* next number ≡ 23 (mod 37) */
        }
    }

    return cache[n - 1];
}
