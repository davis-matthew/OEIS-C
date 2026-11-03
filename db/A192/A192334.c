/*
 * A192334  Number of elements of the finite group defined by the elliptic
 *          curve y^2 = x^3 + x + 1 (mod prime(n)).
 * https://oeis.org/A192334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of F_{prime(n)}-points of the elliptic curve y^2 =
 *            x^3 + x + 1, counting the point at infinity
 * Range:     u8 n <= 51, u16 n <= 6503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8798 (2026-11-01)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses get_prime to obtain the n‑th prime.  A lightweight modular
   exponent routine is implemented to compute Legendre symbols using
   Euler's criterion.  The routine works for any seq_t that can hold
   64‑bit values, keeping intermediate arithmetic in uint64_t to avoid
   arbitrary‑precision allocations. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mulmod.h"

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

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = get_prime(n);
    /* Handle the special case p == 2 separately. */
    if (p == 2U) {
        return (seq_t)3; /* (0,1),(1,0) and the point at infinity */
    }

    uint64_t p64 = (uint64_t)p;
    /* Pre‑compute the exponent for Euler's criterion. */
    uint64_t exp = (p64 - 1ULL) >> 1U;   /* (p-1)/2 */

    /* Start with the point at infinity. */
    uint64_t count = 1ULL;

    for (idx_t x = 0; x < (idx_t)p; ++x) {
        /* Compute RHS = x^3 + x + 1 (mod p). */
        uint64_t xx   = (uint64_t)x * (uint64_t)x % p64;           /* x^2 mod p */
        uint64_t xxx  = (xx * (uint64_t)x) % p64;                  /* x^3 mod p */
        uint64_t rhs  = (xxx + (uint64_t)x + 1ULL) % p64;          /* RHS mod p */

        if (rhs == 0ULL) {
            /* y = 0 is the only solution. */
            count += 1ULL;
            continue;
        }

        /* Legendre symbol via Euler's criterion: rhs^((p-1)/2) mod p. */
        uint64_t leg = mod_pow_uint64(rhs, exp, p64);

        if (leg == 1ULL) {
            /* Two solutions for y. */
            count += 2ULL;
        }
        /* No addition if leg == p-1 (non‑residue). */
    }

    /* Convert to seq_t before returning.  The count never exceeds
       64 bits for the primes encountered in normal use. */
    return (seq_t)count;
}
