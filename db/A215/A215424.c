/*
 * A215424  Primes congruent to {0, 1, 2, 4, 5, 6} mod 13.
 * https://oeis.org/A215424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of OEIS sequence A215424:
 * a(n) = the n-th prime congruent to 0,1,2,4,5,6 (mod 13)
 * (offset 1: a(1)=2).
 * Uses an incremental prime search with caching.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isPrimeTrial.h"

/* Ensure the dynamic array has at least `need` elements. */
static void grow_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 4;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* OEIS sequence A215424 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    static uint64_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 2; /* next integer to test */

    while (cache_len < n) {
        uint64_t cand = next_candidate;
        /* find the next prime with an allowed residue modulo 13 */
        while (1) {
            next_candidate = cand + 1;
            if (isPrimeTrial((seq_t)cand)) {
                uint64_t r = cand % 13;
                if (r == 0 || r == 1 || r == 2 || r == 4 || r == 5 || r == 6)
                    break; /* correct prime found */
            }
            cand = next_candidate;
        }

        /* store found prime */
        grow_uint64(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = cand;
    }

    return (seq_t)cache[n - 1];
}
