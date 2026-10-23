/*
 * A035147  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = -43.
 * https://oeis.org/A035147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-43,d)
 * Range:     u8 n <= 3328, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative with a(43^e)=1, a(p^e)= (1+(-1)^e)/2
   if Kronecker(-43,p)=-1, a(p^e)=e+1 if Kronecker(-43,p)= 1. */

#include <stdint.h>
#include <stdbool.h>

/* Modular exponentiation (base^exp mod mod).  Works for 64‑bit operands
   and returns a value in the same unsigned type as seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) {
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (seq_t)((__uint128_t)result * b % mod);
        b = (seq_t)((__uint128_t)b * b % mod);
        exp >>= 1U;
    }
    return result;
}

/* Kronecker symbol (−43|p) for an odd prime p or p=2.
   p==43 is handled separately outside this function. */
static int kronecker_minus43_prime(uint64_t p) {
    if (p == 2) {
        /* (−43|2) : −43 ≡ 5 ≡ −3 (mod 8) → −1 */
        return -1;
    }
    /* Legendre symbol (a|p) with a = −43 mod p.
       Since p∤43, a≠0. */
    uint64_t a = (p - (43U % (uint32_t)p)) % p; /* (−43 mod p) */
    seq_t res = mod_pow(a, (p - 1) / 2, p);
    if (res == 1)          return 1;
    if (res == p - 1)      return -1;
    /* Should not reach here for prime p>2 */
    return 0;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;      /* safety for indices < 1 (offset 1) */

    seq_t result = 1;
    uint64_t m = n;           /* mutable copy for factorisation */

    /* factor 2 separately */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) { m /= 2; ++e; }
        seq_t fact;
        int k = kronecker_minus43_prime(2);   /* k = -1 for p=2 */
        if (k == -1) {
            fact = (e & 1ULL) ? (seq_t)0 : (seq_t)1;
        } else {   /* should not happen */
            fact = (seq_t)(e + 1);
        }
        result *= fact;
    }

    /* trial division for odd primes up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        uint64_t e = 0;
        while (m % p == 0) { m /= p; ++e; }

        seq_t fact;
        if (p == 43) {
            fact = 1;                 /* a(43^e)=1 for all e */
        } else {
            int k = kronecker_minus43_prime(p);   /* ±1 */
            if (k == -1) {
                fact = (e & 1ULL) ? (seq_t)0 : (seq_t)1;
            } else { /* k == +1 */
                fact = (seq_t)(e + 1);
            }
        }
        result *= fact;
    }

    /* if remaining m > 1, it is a prime factor > sqrt(original m) */
    if (m > 1) {
        uint64_t p = m;
        uint64_t e = 1;          /* exponent is 1 */
        seq_t fact;
        if (p == 43) {
            fact = 1;
        } else {
            int k = kronecker_minus43_prime(p);
            if (k == -1) {
                fact = (e & 1ULL) ? (seq_t)0 : (seq_t)1;
            } else { /* k == +1 */
                fact = (seq_t)(e + 1);
            }
        }
        result *= fact;
    }

    return result;
}
