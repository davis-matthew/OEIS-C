/*
 * A014752  Primes of the form x^2 + 27y^2.
 * https://oeis.org/A014752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p such that p ≡ 1 (mod 3) and 2^((p-1)/3) ≡ 1 (mod
 *            p) (primes representable as x² + 27 y²)
 * Range:     u8 n <= 7, u16 n <= 1078, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* cached sequence values              */
static size_t cache_len   = 0;       /* number of values currently cached   */
static size_t cache_cap   = 0;       /* current capacity of cache array     */
static uint64_t next_candidate = 0;  /* next integer to test (0 = uninitialised) */

/* Ensure that 'cache' has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* no defined a(0); callers may ignore this case */
        return (seq_t)0;

    /* Initialise candidate on first use. */
    if (next_candidate == 0)
        next_candidate = 2ULL;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;

        /* Advance candidate to next odd number after this iteration. */
        if (next_candidate == 2ULL)
            next_candidate = 3ULL;
        else
            next_candidate += 2ULL;

        /* Test primality.  is_prime expects an idx_t (uint64_t). */
        if (!is_prime((idx_t)p))
            continue;

        /* Required condition: p ≡ 1 (mod 3) and 2^((p-1)/3) ≡ 1 (mod p). */
        if ((p % 3ULL) == 1ULL) {
            idx_t exp = (p - 1ULL) / 3ULL;
            if (mod_pow((seq_t)2, exp, (seq_t)p) == (seq_t)1) {
                /* Store the prime in the cache. */
                ensure_cache_capacity();
                cache[cache_len++] = (seq_t)p;
            }
        }
    }

    /* Return the requested term (index n is 1‑based). */
    return cache[n - 1];
}
