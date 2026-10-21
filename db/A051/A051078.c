/*
 * A051078  Primes p such that x^18 = -2 has a solution mod p.
 * https://oeis.org/A051078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, grow_seq_array, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search primes p such that the congruence x^18 ≡ -2 (mod p) is solvable.
   Implements A051078 with caching for arbitrary-order calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq_array.h"
#include "../utils/gcd_u64.h"

/* Euclidean algorithm for 64‑bit unsigned integers */

seq_t a(idx_t n)
{
    /* Static cache of primes satisfying the condition. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_p = 0;      /* last prime found */

    if (n == 0)                /* safety; offset is 1 */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    uint64_t p;
    if (cache_len == 0)
        p = 2;
    else
        p = last_p + 1;               /* start after the last cached prime */

    /* Ensure odd after 2 */
    if (p > 2 && (p & 1) == 0)
        p++;

    /* Generate primes until we have enough. */
    while (cache_len < (size_t)n) {
        if (is_prime_big((seq_t)p)) {
            bool ok = false;

            if (p == 2) {
                /* 0^18 = -2 (mod 2) */
                ok = true;
            } else {
                uint64_t pm1 = p - 1;
                uint64_t d = gcd_u64(pm1, 18ULL);
                uint64_t exp = pm1 / d;

                seq_t base = (seq_t)p - 2;       /* (-2) mod p */
                seq_t r = mod_pow(base, exp, (seq_t)p);

                if (r == (seq_t)1)
                    ok = true;
            }

            if (ok) {
                /* Append to cache */
                grow_seq_array(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)p;
                last_p = p;
            }
        }

        /* Advance to the next candidate (skip evens). */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    return cache[n - 1];
}
