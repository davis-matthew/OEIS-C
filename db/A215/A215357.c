/*
 * A215357  Primes congruent to {0, 2, 3, 4, 5} mod 19.
 * https://oeis.org/A215357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes congruent to {0, 2, 3, 4, 5} modulo 19 (OEIS A215357) */
/* The function returns the n‑th such prime (a(1)=2).  Results are cached so
   that any random access a(n) is fast.  Primality is tested with the
   provided is_prime_small helper. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static seq_t *cache = NULL;          /* cached terms a(1)..a(k) */
static size_t cache_cap = 0;         /* capacity of the cache array */
static size_t cache_len = 0;         /* number of terms stored */
static idx_t next_candidate = 2;     /* smallest integer to try next */

/* Make sure the cache can hold at least `needed` elements. */
static void cache_ensure(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 – caller decides */

    /* Generate terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        /* Find the next integer that matches one of the allowed residues
           and is prime. */
        for (;;) {
            if (next_candidate < 2)
                next_candidate = 2;
            uint8_t r = (uint8_t)(next_candidate % 19ULL);
            if (r == 0 || r == 2 || r == 3 || r == 4 || r == 5) {
                if (is_prime_small((seq_t)next_candidate)) {
                    cache_ensure(cache_len + 1);
                    cache[cache_len++] = (seq_t)next_candidate;
                    ++next_candidate;    /* move past this prime */
                    break;
                }
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
