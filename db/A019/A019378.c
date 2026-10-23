/*
 * A019378  Primes with primitive root 54.
 * https://oeis.org/A019378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 54 is a primitive root modulo
 *            p (OEIS A019378). The function returns a(1) = 7, a(2) = 13, etc.
 *            It searches primes in increasing order and keeps a static cache
 *            so that out‑of‑order calls are handled efficiently
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */

/* Test whether 54 is a primitive root modulo a prime p.  p must
   be > 3 and coprime to 54. */
static bool has_primitive_root_54(uint64_t p)
{
    /* For primes 2 and 3 the exponent ≡ 0 (mod p), so 54 can't be
       a primitive root. */
    if (p <= 3U) return false;

    uint64_t m = p - 1ULL;          /* φ(p) = p‑1 */
    uint64_t d = 2ULL;

    seq_t base = (seq_t)54;
    seq_t mod  = (seq_t)p;
    seq_t one  = (seq_t)1;

    /* Check each distinct prime factor of m. */
    while (d * d <= m) {
        if (m % d == 0ULL) {
            /* Test the exponent (p‑1)/d. */
            uint64_t exp = (p - 1ULL) / d;
            if (mod_pow(base, exp, mod) == one) return false;

            while (m % d == 0ULL) m /= d;
        }
        /* Skip to next divisor: 2 → 3 → 5 → 7 → … */
        d = (d == 2ULL) ? 3ULL : d + 2ULL;
    }

    /* Remaining prime factor greater than √m (if any). */
    if (m > 1ULL) {
        uint64_t exp = (p - 1ULL) / m;
        if (mod_pow(base, exp, mod) == one) return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 */

    /* Static cache of found primes. */
    static seq_t *found   = NULL;
    static idx_t count    = 0;
    static idx_t capacity = 0;
    static uint64_t next_candidate = 5;   /* start searching from 5 */

    /* Grow the cache until it contains the n-th prime with the property. */
    while (count < n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate))
            next_candidate += 2;   /* skip even numbers */

        /* Test the candidate for the primitive root property. */
        if (has_primitive_root_54(next_candidate)) {
            /* Expand the cache if necessary. */
            if (capacity <= count) {
                capacity = capacity ? (capacity * 2) : 4;
                if (capacity < count + 1) capacity = count + 1;
                found = (seq_t *)realloc(found, capacity * sizeof(seq_t));
            }
            found[count++] = (seq_t)next_candidate;
        }

        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }

    return found[n - 1];
}
