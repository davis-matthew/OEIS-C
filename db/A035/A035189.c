/*
 * A035189  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          7.
 * https://oeis.org/A035189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(7,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_7_of_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative implementation based on prime factorisation. */
/* For an odd prime p ≠ 7, Kronecker(7,p)= (p|7)*(-1)^((p-1)/2). */
/* For p = 7, the factor contributes 1 (a(7^e)=1). */
/* For p = 2, Kronecker(7,2)=1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/kronecker_7_of_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n=0, but for safety */

    uint64_t m = (uint64_t)n;
    seq_t res = 1;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        res *= (e + 1);               /* Kronecker(7,2) = 1 → factor e+1 */
    }

    /* Trial division for odd primes up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        if (p == 7) {
            /* factor 7^e contributes 1, so ignore */
            continue;
        }

        int kr = kronecker_7_of_prime(p);
        if (kr == 1) {
            res *= (e + 1);   /* a(p^e) = e+1 */
        } else { /* kr == -1 */
            if (e & 1)        /* odd exponent → zero factor */
                return 0;
            /* even exponent → factor 1, no change to res */
        }
    }

    /* Remaining prime factor greater than sqrt(original n) */
    if (m > 1) {
        uint64_t p = m;
        if (p == 7) {
            /* ignore */
        } else if (p == 2) {
            res *= 2;        /* exponent 1 ⇒ e+1 = 2 */
        } else {
            int kr = kronecker_7_of_prime(p);
            if (kr == 1) {
                res *= 2;    /* exponent 1 ⇒ e+1 = 2 */
            } else {
                /* kr == -1 and exponent 1 is odd → zero */
                return 0;
            }
        }
    }

    return res;
}
