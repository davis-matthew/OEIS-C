/*
 * A142434  Primes congruent to 24 mod 49.
 * https://oeis.org/A142434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime ≡ 24 (mod 49) */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found primes       */
    static size_t cap   = 0;         /* current capacity of cache   */
    static idx_t found  = 0;         /* number of primes stored     */
    static seq_t last_checked = 24;  /* last tested candidate       */

    /* Find more primes until we have at least n of them. */
    while (found < n) {
        last_checked += (seq_t)49;   /* next candidate ≡ 24 (mod 49) */
        /* use the provided small‑prime test (fits 64 bit values) */
        if (is_prime_small((uint64_t)last_checked)) {
            /* grow cache if necessary */
            if (found == (idx_t)cap) {
                ensure_capacity(found + 1, &cap, (void **)&cache, sizeof(seq_t));
            }
            cache[found++] = last_checked;
        }
    }
    return cache[n - 1];  /* 1‑based indexing in the OEIS sequence */
}
