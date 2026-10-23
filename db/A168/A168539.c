/*
 * A168539  Terms of A123239 which are prime in Z(i), Z(rho) and Z(sqrt(2)).
 * https://oeis.org/A168539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime congruent to 11 (mod 24)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (these are the primes that remain inert in the rings Z(i), Z[ρ] and Z[√2]) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached qualifying primes */
static size_t cache_cap = 0;         /* current allocation size   */
static size_t cache_len = 0;         /* number of stored primes    */
static uint64_t last_checked = 1;    /* last candidate examined */

/* enlarge cache until it holds at least n elements */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;          /* nothing to do */

    if (cache_len >= (size_t)n) return;

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        /* get next candidate congruent to 11 (mod 24) */
        uint64_t cand = last_checked + 1;
        uint64_t rem = cand % 24;
        if (rem <= 11)
            cand += 11 - rem;
        else
            cand += 24 - rem + 11;

        /* test primality */
        if (is_prime_big((seq_t)cand)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)cand;
            cache_len++;
        }

        last_checked = cand;     /* continue from here */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */
    ensure_cache(n);
    return cache[n - 1];
}
