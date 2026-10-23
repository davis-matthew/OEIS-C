/*
 * A035224  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = 42.
 * https://oeis.org/A035224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(42,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small helper: Legendre symbol (a|p) for odd prime p. */
static int legendre_mod(uint64_t a, uint64_t p)
{
    uint64_t exp = (p - 1) >> 1;
    uint64_t base = a % p;
    uint64_t res  = 1;
    while (exp > 0) {
        if (exp & 1)
            res = (uint64_t)((__uint128_t)res * base % p);
        base = (uint64_t)((__uint128_t)base * base % p);
        exp >>= 1;
    }
    if (res == 1)          return 1;
    if (res == p - 1)      return -1;
    return 0;             /* should not happen for odd prime p */
}

/* Kronecker symbol (42|p) for odd prime p (p!=3,7). */
static int kronecker_42_prime(uint64_t p)
{
    /* Skip if p divides 42 – handled separately */
    if ((p & 1ULL) == 0 || p == 3ULL || p == 7ULL)
        return 0;  /* Kronecker(42,p)=0 when p even or 3 or 7 */

    int s = 1;

    /* (2|p) */
    uint64_t r = p % 8ULL;
    int k2 = (r == 1 || r == 7) ? 1 : -1;
    s *= k2;

    /* (3|p) */
    uint64_t r3 = p % 12ULL;
    int k3 = (r3 == 1 || r3 == 11) ? 1 : -1;
    s *= k3;

    /* (7|p) */
    int k7 = legendre_mod(7ULL, p);
    s *= k7;

    return s;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t  res = 1;

    /* Factor 2 separately – its Kronecker value is 0, contributes a factor 1. */
    while ((m & 1ULL) == 0) m >>= 1;

    /* Trial division for odd primes up to sqrt(m). */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            int kr = kronecker_42_prime(p);
            if (kr == 0)
                continue;
            else if (kr == -1) {
                if ((e & 1) != 0)
                    return 0;
                /* even exponent → factor 1, nothing to multiply */
            } else { /* kr == 1 */
                res *= (seq_t)(e + 1);
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        /* m is a prime > sqrt(original n) */
        int kr = kronecker_42_prime(m);
        if (kr == 0) {
            /* factor 1 */
        } else if (kr == -1) {
            /* exponent e = 1, odd → result zero */
            return 0;
        } else { /* kr == 1 */
            res *= (seq_t)2;
        }
    }

    return res;
}
