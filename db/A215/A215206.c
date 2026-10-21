/*
 * A215206  Primes congruent to {2, 7} mod 11.
 * https://oeis.org/A215206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A215206 – Primes congruent to {2, 7} mod 11
   a(n) is the n‑th prime p with p % 11 == 2 or 7, n ≥ 1.
   The function builds the list lazily, storing found primes in a
   static array that is tall enough for n ≈ 10 000.  */
/* (The algorithm simply tests consecutive integers for primality and
   keeps those with the proper residue.  It works for any seq_t that
   supports the usual arithmetic and assignment operations.) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

#define MAX_CACHE 200000     /* enough for n ≈ 10 000 */

static seq_t cache[MAX_CACHE];   /* holds qualifying primes */
static size_t cache_cnt = 0;      /* number of primes stored */
static uint64_t next_candidate = 2;  /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for non‑positive indices */

    /* Grow the cache until it contains at least n primes. */
    while (cache_cnt < (size_t)n) {
        uint64_t rem = next_candidate % 11;
        if (rem == 2 || rem == 7) {
            if (isPrimeTrial((seq_t)next_candidate)) {
                if (cache_cnt < MAX_CACHE) {
                    cache[cache_cnt++] = (seq_t)next_candidate;
                }
                /* If the cache is full, we simply keep generating but
                   cannot store more – this situation should never
                   occur for the intended n values. */
            }
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
