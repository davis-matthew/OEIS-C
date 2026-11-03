/*
 * A178813  a(n) = (prime(n)^(p-1) - 1)/p^2 mod p, where p is the first prime
 *          that divides (prime(n)^(p-1) - 1)/p.
 * https://oeis.org/A178813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = (prime(n)^(p-1) - 1)/p^2 mod p, where p is the first
 *            prime such that p^2 | prime(n)^(p-1) - 1
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* fast modular exponentiation for 64‑bit integers, using 128‑bit multiplication */
static uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1U)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1U;
    }
    return result;
}

/* Find the first prime p such that p^2 divides base^(p-1) - 1   */
static uint64_t first_wieferich(uint64_t base)
{
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime(p))
            continue;
        if (pow_mod_u64(base, p - 1, p * p) == 1U)
            return p;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;                /* out‑of‑range requests */

    uint64_t base = (uint64_t)get_prime(n);   /* prime(n) */
    uint64_t p    = first_wieferich(base);    /* Wieferich prime for this base */

    uint64_t mod_p3 = p * p * p;              /* p³ fits in 64‑bit for the limits needed */
    uint64_t pow_mod = pow_mod_u64(base, p - 1, mod_p3);

    /* (base^(p-1)-1)/p² modulo p.  Since base^(p-1) ≡ 1 (mod p²),
       pow_mod = 1 + k·p² with k < p.  Thus ((pow_mod-1)/p²) % p = k.  */
    uint64_t quotient = (pow_mod - 1U) / (p * p);   /* quotient < p */

    return (seq_t)quotient;              /* quotient already < p */
}
