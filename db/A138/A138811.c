/*
 * A138811  Theta series of quadratic form x^2 + x*y + 11*y^2.
 * https://oeis.org/A138811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=2 * ∏_{p^e∥n, p≠43} f(p,e) where
 *            f(p,e)=e+1 if Kronecker(-43,p)=1, 1 if Kronecker(-43,p)=-1 and e
 *            even, 0 if Kronecker(-43,p)=-1 and e odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative; the product runs over prime powers of n. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/powmod64.h"   /* powmod64(uint64_t, uint64_t, uint64_t) */

static int legendre_43(uint64_t p)
/* returns (43|p), Legendre symbol for odd prime p. */
{
    uint64_t a = 43ULL % p;
    uint64_t res = powmod64(a, (p - 1) / 2, p);
    if (res == 1) return 1;
    if (res == p - 1) return -1;
    return 0;   /* never happens for a prime p */
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0)=1 */

    uint64_t x = (uint64_t)n;
    seq_t prod = 1;                     /* product of factors */

    /* handle factor 2 separately */
    if ((x & 1ULL) == 0) {
        uint64_t e = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++e;
        }
        /* (-43|2) = -1, so factor contributes 1 if e even, 0 if e odd */
        if (e & 1ULL) return 0;
        /* otherwise contribution is 1, nothing to multiply */
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (p == 43ULL) continue;   /* b(43^e)=1, ignored */

            int leg = legendre_43(p);   /* (43|p) */
            int kr = ((p & 3ULL) == 1ULL) ? leg : -leg;  /* (-43|p) */

            if (kr == 1) {
                prod *= (seq_t)(e + 1);
            } else { /* kr == -1 */
                if (e & 1ULL) return 0;  /* odd exponent gives factor 0 */
                /* even exponent gives factor 1, nothing to multiply */
            }
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        uint64_t p = x;
        uint64_t e = 1;
        if (p == 43ULL) /* skip 43^e, because b(43^e)=1 */
            ;
        else {
            int leg = legendre_43(p);   /* (43|p) */
            int kr = ((p & 3ULL) == 1ULL) ? leg : -leg;  /* (-43|p) */

            if (kr == 1) {
                prod *= (seq_t)(e + 1); /* e is 1 */
            } else { /* kr == -1 */
                if (e & 1ULL) return 0; /* odd exponent -> factor 0 */
            }
        }
    }

    return prod * 2;  /* a(n) = 2 * b(n), except for n=0 handled above */
}
