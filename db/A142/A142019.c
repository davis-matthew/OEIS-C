/*
 * A142019  Primes congruent to 15 mod 31.
 * https://oeis.org/A142019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 15 mod 31
 * Range:     u8 n <= 1, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 938 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t cand = 15;  /* last tested candidate (initially 15) */

seq_t a(idx_t n)
{
    /* Lazy allocation of cache */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;      /* allocation failure handled gracefully */
    }

    /* Extend the cache until it contains at least n elements */
    while (cache_len < n) {
        cand += 31;                    /* next number of the form 15 mod 31 */
        if (is_prime_seq(cand)) {    /* test primality */
            if (cache_len == cache_cap) { /* grow the cache if needed */
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = cand;  /* store the found prime */
        }
    }

    return cache[n - 1];               /* zero‑based array index */
}
