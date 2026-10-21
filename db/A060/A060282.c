/*
 * A060282  Periodic part of decimal expansion of reciprocal of n-th prime
 *          (leading 0's omitted).
 * https://oeis.org/A060282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   _factorize_distinct, get_prime, mul_mod64, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the periodic part of 1/prime(n), OEIS A060282
   a(n) = floor(10^τ(n) / prime(n)),  τ(n) = length of the repetend
   of 1/prime(n).  The function uses the nth prime directly (1‑indexed).  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"      /* get_prime(k): kth prime, 1‑indexed */
#include "../utils/mul_small.h"      /* mul_small(seq_t a, uint32_t m) */
#include "../utils/mul_mod64.h"      /* mul_mod64(uint64_t a, uint64_t b, uint64_t m) */
#include "../utils/_factorize_distinct.h"

/* Fast modular exponentiation (modulo m) using mul_mod64 to avoid overflow. */
static uint64_t
_modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Multiplicative order of 10 modulo a prime p (p ≠ 2,5).  Returns the
   period length τ(p). */
static uint64_t
_order_of_10(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t factors[64];
    int fcnt = _factorize_distinct(phi, factors);
    uint64_t order = phi;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0 &&
               _modpow64(10, order / q, p) == 1)
            order /= q;
    }
    return order;
}

/* Public entry point – returns the periodic part for the nth prime. */
seq_t
a(idx_t n)
{
    if (n == 0)           /* below the OEIS offset – undefined behaviour */
        return 0;

    /* n is 1‑indexed: get_prime(1) = 2, get_prime(2) = 3, … */
    uint64_t p = (uint64_t)get_prime(n);

    /* Primes 2 and 5 have terminating decimal expansions. */
    if (p == 2 || p == 5)
        return 0;

    /* Length of the repetend. */
    uint64_t L = _order_of_10(p);

    /* Build the integer made of the L digits of the period. */
    seq_t result = 0;
    uint64_t remainder = 1;
    for (uint64_t i = 0; i < L; ++i) {
        remainder *= 10ULL;
        seq_t digit = (seq_t)(remainder / p);
        remainder   = remainder % p;
        result = mul_small(result, 10) + digit;
    }
    return result;
}
