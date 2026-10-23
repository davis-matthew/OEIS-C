/*
 * A105876  Primes for which -4 is a primitive root.
 * https://oeis.org/A105876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A105876 – Primes for which –4 is a primitive root.
   The function tests odd primes p in increasing order and
   keeps those for which for every prime divisor q of p‑1
   the congruence (-4)^((p-1)/q) mod p ≠ 1 holds.     */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* is_prime(uint64_t) -> bool */

/* Modular exponentiation for 64‑bit integers. */
static uint64_t
modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* Test whether -4 is a primitive root modulo the odd prime p. */
static bool
is_minus4_primitive(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t base = (p >= 4) ? p - 4 : p + (p - 4);   /* base ≡ –4 (mod p) */

    uint64_t m = phi;
    for (uint64_t q = 2; q * q <= m; ++q) {
        if (m % q == 0) {
            while (m % q == 0) m /= q;           /* remove all copies of q */
            if (modpow_uint64(base, phi / q, p) == 1)
                return false;
        }
    }
    if (m > 1) {   /* remaining prime factor */
        if (modpow_uint64(base, phi / m, p) == 1)
            return false;
    }
    return true;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined for n < 1 */

    static seq_t *primes = NULL;   /* dynamic array of found primes      */
    static idx_t primes_len = 0;   /* number of primes stored            */
    static idx_t primes_cap = 0;   /* allocated capacity                 */
    static uint64_t next_candidate = 3; /* current odd candidate            */

    /* Ensure the array has room for at least n primes. */
    while (primes_len < n) {

        /* Ensure odd candidate; skip the even number 2. */
        if ((next_candidate & 1) == 0)
            ++next_candidate;

        /* Skip non‑primes. */
        if (!is_prime(next_candidate)) {
            next_candidate += 2;
            continue;
        }

        /* Check the primitive‑root property. */
        if (is_minus4_primitive(next_candidate)) {
            /* Grow the array if necessary. */
            if (primes_len == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 128 : primes_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp)  /* allocation failure – abort */
                    exit(1);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = (seq_t)next_candidate;
        }

        next_candidate += 2;   /* advance to the next odd number */
    }

    return primes[n - 1];
}
