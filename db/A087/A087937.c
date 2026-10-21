/*
 * A087937  Prime hypotenuse of primitive Pythagorean triangles with even
 *          short leg (or odd long leg).
 * https://oeis.org/A087937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1623, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_u64, isqrt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches primes ≡1 (mod 4) and keeps those that can be written as
   p = m² + n² with m>n>0, m n coprime and opposite parity, and
   satisfy 2mn < m²‑n².  The resulting list is memoised so that
   calls to a(n) are O(1) after the first call.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility for primality testing (available in the build). */
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt64.h"

/* Cached sequence values. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_cnt = 0;

/* Next odd number to test for primality. */
static uint64_t next_candidate = 5;

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; n==0 is undefined – return 0 */
        return (seq_t)0;

    /* Initialise the cache on first use. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n entries. */
    while (cache_cnt < n) {
        uint64_t p;
        /* Find the next odd prime that is ≡1 (mod 4). */
        for (;;) {
            p = next_candidate;
            next_candidate += 2;
            if ((p & 3u) == 1u && is_prime_u64(p))
                break;
        }

        /* Find the representation p = m² + n² (unique for primes ≡1 mod 4). */
        uint64_t m = 0, n_val = 0;
        bool found = false;
        uint64_t limit = isqrt64(p >> 1);   /* n ≤ sqrt(p/2) */
        for (uint64_t t = 1; t <= limit; ++t) {
            uint64_t t2 = t * t;
            uint64_t rem = p - t2;
            uint64_t msq = t2 + rem;       /* same as p, for clarity */
            (void)msq;                     /* silence unused warning */
            uint64_t sqrt_rem = isqrt64(rem);
            if (sqrt_rem * sqrt_rem == rem) {
                m = sqrt_rem;
                n_val = t;
                found = true;
                break;
            }
        }
        if (!found) continue;                 /* should never happen for p≡1 mod 4 */

        /* Check the short‑leg condition: 2mn < m²‑n². */
        if (2ULL * m * n_val >= m * m - n_val * n_val)
            continue;

        /* Store the prime hypotenuse in the cache. */
        if (cache_cnt == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
        }
        cache[cache_cnt++] = (seq_t)p;
    }

    return cache[n - 1];
}
