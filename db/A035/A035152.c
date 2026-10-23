/*
 * A035152  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -38.
 * https://oeis.org/A035152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∑_{d|n} Kronecker(-38,d) The function implements this by
 *            using the multiplicative property of the Kronecker symbol. For a
 *            prime power p^e, a(p^e) = 1 if Kronecker(-38,p) = 0 a(p^e) =
 *            (1+(-1)^e)/2 ( = 1 if e even, 0 if e odd) if Kronecker(-38,p) =
 *            -1 a(p^e) = e+1 if Kronecker(-38,p) = 1 The...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_minus38_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/kronecker_minus38_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 0 is not used; index 1 is the first term. */
    if (n == 0)
        return 0;

    seq_t result = 1;
    uint64_t tmp   = (uint64_t)n;          /* copy into native type */

    /* Remove the factor 2 (Kronecker(-38,2)=0, contributes only 1) */
    while ((tmp & 1ULL) == 0)
        tmp >>= 1;

    /* Factor odd primes by trial division */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }

            int kr = kronecker_minus38_prime(p);
            if (kr == 0) {
                /* multiplicative factor 1 */
                continue;
            } else if (kr == -1) {
                if (exp & 1) {          /* odd exponent → factor 0 */
                    return 0;
                }
                /* even exponent → factor 1, nothing to do */
            } else { /* kr == 1 */
                result *= (seq_t)(exp + 1);
            }
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1) {
        int kr = kronecker_minus38_prime(tmp);
        if (kr == 0) {
            /* factor 1 */
        } else if (kr == -1) {
            /* exponent 1 (odd) => result 0 */
            return 0;
        } else { /* kr == 1 */
            result *= (seq_t)2; /* exponent 1 ⇒ e+1 = 2 */
        }
    }

    return result;
}
