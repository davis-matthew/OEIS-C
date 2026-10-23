/*
 * A040994  Primes p such that x^6 = 3 has a solution mod p.
 * https://oeis.org/A040994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^6 ≡ 3 (mod p) has a solution */

/* The sequence is computed by testing primes in increasing order and
   keeping those for which 3 is a sixth‑power residue modulo p.
   The test for a prime p>3 is:
          g = gcd(6, p-1)
          if 3^((p-1)/g) ≡ 1 (mod p) then p is accepted.
   For p=2 or 3 the equation trivially has a solution. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, return 0 to avoid UB */

    static seq_t *cache = NULL;   /* cache of discovered primes */
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t next_val = 2; /* next integer to test for primality */

    /* Grow the cache if we need more terms */
    while (cache_size < (size_t)n) {
        /* Ensure the cache buffer is big enough */
        ensure_cache(n, &cache, &cache_cap);

        uint64_t p = next_val;
        ++next_val;

        if (!is_prime(p)) continue;   /* skip composites */

        bool ok = false;
        if (p == 2 || p == 3) {
            ok = true;                 /* 2 and 3 always satisfy the congruence */
        } else {
            seq_t g = gcd((seq_t)6, (seq_t)(p - 1));
            /* g fits in seq_t, but we need it as a divisor of (p-1) */
            uint64_t div = (uint64_t)g;
            uint64_t exp = (p - 1) / div;
            seq_t val = mod_pow((seq_t)3, exp, (seq_t)p);
            ok = (val == (seq_t)1);
        }

        if (ok) {
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
