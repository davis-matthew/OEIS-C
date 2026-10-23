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
 * Helpers:   get_prime, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses get_prime to obtain the n‑th prime.  A lightweight modular
   exponent routine is implemented to compute Legendre symbols using
   Euler's criterion.  The routine works for any seq_t that can hold
   64‑bit values, keeping intermediate arithmetic in uint64_t to avoid
   arbitrary‑precision allocations. */

#include <stdint.h>
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */
#include "../utils/mul_mod64.h"   /* uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod); */

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
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
