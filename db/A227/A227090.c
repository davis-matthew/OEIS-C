/*
 * A227090  Numbers k such that k^2+1 with its digits reversed is prime
 *          (ignoring leading zeros).
 * https://oeis.org/A227090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 60, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   reverse_decimal_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A227090: numbers k such that k^2+1 with its digits reversed is prime.
   The implementation performs an on‑demand search, caching found terms. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_decimal_uint64.h"

/* Local helper: test primality of a 64‑bit unsigned integer (deterministic). */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (should not happen in valid tests). */
    if (n == 0) return 0;

    /* Static state shared across all calls. */
    static seq_t *cache = NULL;      /* array of discovered terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t last_checked = 0;   /* largest integer examined so far */

    /* Fill the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Next candidate k. */
        ++last_checked;

        /* Compute k^2 + 1. Because last_checked fits in seq_t, the
           multiplication and addition are performed in seq_t arithmetic. */
        seq_t sq = (seq_t)last_checked * last_checked + 1;

        /* Reverse the decimal digits.  Because (k^2+1) is very small
           (for the required n), it safely fits in a 64‑bit unsigned int. */
        uint64_t sq64 = (uint64_t)sq;
        uint64_t rev64 = reverse_decimal_uint64(sq64);

        /* Test primality of the reversed number. */
        if (is_prime_u64(rev64)) {
            /* Expand cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(1); }   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* Return the requested term (1‑based indexing). */
    return cache[n - 1];
}
