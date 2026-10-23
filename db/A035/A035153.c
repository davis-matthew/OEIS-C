/*
 * A035153  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -37.
 * https://oeis.org/A035153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-37,d) Multiplicative definition:
 *            a(37^e) = 1 a(p^e) = (1+(-1)^e)/2 if Kronecker(-37,p) = -1
 *            a(p^e) = e + 1 if Kronecker(-37,p) = +1 with Kronecker(-37,p)
 *            computed via the Jacobi symbol
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

static inline int kronecker_minus37_prime(uint64_t p)
{
    /* returns +1 or -1 for the Kronecker symbol (-37/p) */
    if (p == 2ULL)  /* (-37/2) = -1 for -37 ≡ 3 (mod 8) */
        return -1;
    /* Jacobi(-1,p) = (-1)^((p-1)/2) */
    int sign = ((p & 3ULL) == 3ULL) ? -1 : 1;   /* (−1/p) */
    int legendre_37 = jacobi_symbol(37ULL, p);   /* (37/p) */
    return sign * legendre_37;                  /* (-37/p) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for OEIS, safe guard */
    idx_t t = n;
    seq_t result = 1;

    /* factor out powers of 2 */
    idx_t e = 0;
    while ((t & 1ULL) == 0) {        /* t % 2 == 0 */
        e++;
        t >>= 1;
    }
    if (e > 0) {
        /* Kronecker(-37,2) = -1; contribution = (1+(-1)^e)/2 */
        seq_t contrib = ((e & 1ULL) == 0) ? 1 : 0;
        result *= contrib;
        if (result == 0) return 0;
    }

    /* trial division for odd primes */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p != 0) continue;

        e = 0;
        while (t % p == 0) {
            e++;
            t /= p;
        }

        seq_t contrib;
        if (p == 37ULL) {
            contrib = 1;
        } else {
            int kr = kronecker_minus37_prime(p);   /* +1 or -1 */
            if (kr == 1) {
                contrib = (seq_t)(e + 1);
            } else { /* kr == -1 */
                contrib = ((e & 1ULL) == 0) ? 1 : 0;
            }
        }
        result *= contrib;
        if (result == 0) return 0;
    }

    /* remaining prime factor, if any */
    if (t > 1) {
        idx_t p = t;
        if (p == 37ULL) {
            result *= 1;
        } else {
            int kr = kronecker_minus37_prime(p);
            if (kr == 1) {
                result *= 2;   /* e = 1 => e+1 = 2 */
            } else {
                result *= 0;   /* e = 1 => (1+(-1))/2 = 0 */
                return 0;
            }
        }
    }

    return result;
}
