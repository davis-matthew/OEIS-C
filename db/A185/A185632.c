/*
 * A185632  Primes of the form n^2 + n + 1 where n is nonprime.
 * https://oeis.org/A185632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A185632: primes of the form n^2 + n + 1 with n a non‑prime.
 *
 *  The function returns the n‑th term (1‑based) of that sequence.
 *  It builds the values on demand and caches them to allow
 *  non‑sequential calls efficiently.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"

/*  Public entry point (seq_t and idx_t are provided by the build).  */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1; return 0 to avoid UB */
        return (seq_t)0;

    /* Static cache of computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Next integer n to test (1‑based).  */
    static idx_t next_candidate = 1;

    /* Extend the cache until it covers the requested index. */
    while (cache_len < n) {
        /* Skip prime n's. */
        while (is_prime(next_candidate))
            ++next_candidate;

        /* Compute p = n^2 + n + 1.  Use 128‑bit arithmetic to avoid overflow. */
        __uint128_t nn = (unsigned __int128)next_candidate;
        __uint128_t p128 = nn * nn + nn + 1;

        /* If the value fits in a 64‑bit integer, test it for primality. */
        if (p128 <= ( (__uint128_t)1 << (sizeof(idx_t) * 8) ) - 1) {
            idx_t p = (idx_t)p128;
            if (is_prime(p)) {   /* p is prime */
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { free(cache); exit(EXIT_FAILURE); }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
