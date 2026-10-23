/*
 * A035186  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          3.
 * https://oeis.org/A035186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(3,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for out-of-range calls */
    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    /* factor the prime 2 */
    if ((tmp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        if (e & 1ULL)          /* odd exponent → zero contribution */
            return 0;
        /* even exponent → contribution 1, no change to result */
    }

    /* factor odd primes up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (p == 3)                /* (3|3)=0 ⇒ contribution 1 */
                continue;

            /* Legendre symbol (3|p):  +1 if p ≡ ±1 (mod 12), –1 otherwise */
            int sign = ((p % 12 == 1 || p % 12 == 11) ? 1 : -1);

            if (sign == 1) {
                result *= (seq_t)(e + 1);           /* e+1 factors */
            } else {                              /* sign == -1 */
                if ((e & 1ULL) == 0)               /* even exponent → 1 */
                    /* nothing */;
                else
                    return 0;                      /* odd exponent → 0 */
            }
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        uint64_t p = tmp;
        if (p == 3) {
            /* contribution 1; nothing to do */
        } else {
            int sign = ((p % 12 == 1 || p % 12 == 11) ? 1 : -1);
            if (sign == 1) {
                result *= (seq_t)(2);               /* e = 1 → e+1 = 2 */
            } else {
                return 0;
            }
        }
    }

    return result;
}
