/*
 * A201820  Numbers k such that 90*k + 23 is prime.
 * https://oeis.org/A201820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201820: a(n) = (p_n - 23) / 90 where p_n is the n‑th prime
   congruent to 23 (mod 90).  a(n) is the corresponding k such that
   90*k + 23 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;      /* cached k values */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* current allocation capacity */
static idx_t cur_candidate = 0;  /* next k to test */

/* Return the n-th value a(n), 1‑based. */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < n) {
        /* Number 90*k + 23 to test for primality. */
        seq_t candidate = (seq_t)cur_candidate * 90 + 23;

        if (isPrimeGeneric(candidate)) {
            if (cache_len == cache_cap) {
                /* Grow the cache to accommodate another entry. */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)cur_candidate;
        }
        /* Move to next k. */
        cur_candidate++;
    }
    return cache[n - 1];
}
