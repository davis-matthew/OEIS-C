/*
 * A077197  a(n) = lambda(phi(n)) where lambda(n) (A002322) is the Carmichael
 *          lambda function which gives the smallest integer m such that k^m =
 *          1 mod n for all integers k relatively prime to n and phi(n) is
 *          A000010.
 * https://oeis.org/A077197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lambda(phi(n)) where lambda is the Carmichael lambda
 *            function
 * Range:     u8 n <= 561, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8506 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Compute φ(n) where n is an idx_t (uint64_t).  The result fits in idx_t. */
static seq_t phi_idx(idx_t n) {
    idx_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return (seq_t)result;
}

/* Euclid's algorithm for gcd of two uint64_t values. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute Carmichael lambda of an unsigned 64‑bit integer.
   λ(1) = 1.  For n > 1, λ(n) = lcm( λ(p^e) over the prime powers of n ).
   λ(p^e) = 2^{e-2}          for p = 2 and e ≥ 3
   λ(2)   = 1
   λ(4)   = 2
   λ(p^e) = p^{e-1} (p – 1)  for odd prime p.                          */
static uint64_t lambda_u64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t result = 1;
    uint64_t m = n;

    /* factor 2 separately for efficiency */
    if ((m & 1ULL) == 0) {
        unsigned e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        uint64_t lambda_2e;
        if (e == 1)       lambda_2e = 1;
        else if (e == 2)  lambda_2e = 2;
        else              lambda_2e = 1ULL << (e - 2); /* 2^(e-2) */
        uint64_t g = gcd_u64(result, lambda_2e);
        result = result / g * lambda_2e;
    }

    /* trial division for remaining odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* λ(p^e) = p^{e-1} * (p-1)  for odd p */
            uint64_t lambda_pe = 1;
            for (unsigned i = 1; i < e; ++i) lambda_pe *= p; /* p^{e-1} */
            lambda_pe *= (p - 1);
            uint64_t g = gcd_u64(result, lambda_pe);
            result = result / g * lambda_pe;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        uint64_t p = m;          /* exponent is 1 */
        uint64_t lambda_p = p - 1;
        uint64_t g = gcd_u64(result, lambda_p);
        result = result / g * lambda_p;
    }

    return result;
}

/* -------------------------------------------------------------
   Internal helpers
   ------------------------------------------------------------- */

/* -------------------------------------------------------------
   Sequence function
   ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* φ(n) is returned as seq_t by the utility function. */
    seq_t phi_val = phi_idx(n);

    /* λ(φ(n)) */
    uint64_t lambda_val = lambda_u64((uint64_t)phi_val);

    return (seq_t)lambda_val;
}
