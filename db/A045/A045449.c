/*
 * A045449  Primes congruent to {4, 5, 6} mod 7.
 * https://oeis.org/A045449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p with p ≡ 4, 5, or 6 (mod 7)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cached list of primes satisfying the residue condition. */
    static uint64_t *cache   = NULL;   /* array of uint64_t primes */
    static idx_t    size    = 0;        /* number of primes stored   */
    static idx_t    cap     = 0;        /* allocated capacity        */
    static idx_t    last    = 0;        /* last integer tested       */

    /* Ensure there is room for at least one new element. */
    if (size >= cap) {
        idx_t new_cap = (cap == 0) ? 8 : (cap << 1);
        uint64_t *tmp  = (uint64_t *)realloc(cache,
                                            (size_t)new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Grow the cache until it contains at least n matching primes. */
    while (size < n) {
        ++last;
        if (!is_prime(last)) continue;          /* skip non‑primes */
        uint64_t mod = last % 7ULL;
        if (mod == 4ULL || mod == 5ULL || mod == 6ULL) {
            cache[size++] = (uint64_t)last;
            /* Reserve space for the next element (if needed). */
            if (size >= cap) {
                idx_t new_cap = cap << 1;
                uint64_t *tmp  = (uint64_t *)realloc(cache,
                                                    (size_t)new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
        }
    }

    /* Convert the stored 64‑bit value to the requested seq_t type. */
    return (seq_t)cache[n - 1];
}
