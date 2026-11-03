/*
 * A158035  a(n) = 2*A158034(n) + 1, prime numbers p for which f = (2^p -
 *          2^((p - 1) / 2 + 1) + 4p^2 - 8p) / (2p^2 - 2p) is an integer.
 * https://oeis.org/A158035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A158034(n) + 1, where A158034(n) is the nth prime p for
 *            which (2^p - 2^((p-1)/2 + 1) + 4p^2 - 8p) / (2p^2 - 2p) is an
 *            integer
 * Range:     u8 n <= 4, u16 n <= 241, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 856 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Iterator of modular exponentiation with 128‑bit arithmetic.
   It can handle moduli larger than the target seq_t. */
static inline __int128 modpow128(__int128 base, uint64_t exp, __int128 mod)
{
    __int128 result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Return the n‑th prime that satisfies the property.
   Caching is used so that subsequent calls are O(1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 1 */

    /* static state */
    static seq_t *good = NULL;       /* array of valid primes */
    static size_t good_cnt = 0;      /* count of stored primes   */
    static size_t good_cap = 0;      /* allocated capacity      */
    static uint64_t candidate = 3;   /* next odd integer to test */

    /* Grow array if needed */
    if (good == NULL) {
        good_cap = 8;
        good = mem_new(seq_t, good_cap);
        if (!good) exit(EXIT_FAILURE);
    }

    /* Find primes until we have n of them */
    while (good_cnt < n) {
        /* Skip non‑primes */
        while (!is_prime(candidate)) {
            candidate += 2;           /* only odd candidates */
        }

        uint64_t p = candidate;
        __int128 p128 = p;
        __int128 denom = 2 * p128 * (p128 - 1);          /* 2p(p-1) */

        /* Compute numerator modulo denom */
        __int128 term1 = modpow128(2, p, denom);         /* 2^p           */
        uint64_t exp2 = (p - 1) / 2 + 1;                 /* (p-1)/2+1     */
        __int128 term2 = modpow128(2, exp2, denom);      /* 2^exp2        */
        __int128 term3 = (4 * p128 * p128 - 8 * p128) % denom;
        __int128 numerator = (term1 - term2 + term3) % denom;
        if (numerator < 0)
            numerator += denom;

        /* Accept if numerator divisible by denom */
        if (numerator == 0) {
            if (good_cnt == good_cap) {                 /* grow array    */
                good_cap <<= 1;
                good = mem_resize(seq_t, good, good_cap);
                if (!good) exit(EXIT_FAILURE);
            }
            good[good_cnt++] = (seq_t)p;
        }

        candidate += 2;                               /* next odd integer */
    }

    return good[n - 1];
}
