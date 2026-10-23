/*
 * A035210  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          28.
 * https://oeis.org/A035210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(28,d) a(n) is multiplicative. For an
 *            odd prime p ≠ 2,7: a(p^e) = e+1 if (7|p) = +1 a(p^e) =
 *            (1+(-1)^e)/2 if (7|p) = -1 For p = 2 or 7, a(p^e) = 1 The
 *            function below factors n and applies these rules
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre7.h"

seq_t a(idx_t n)
{
    uint64_t tmp = (uint64_t)n;
    seq_t res = 1U;   /* multiplicative identity */

    /* Count power of 2 */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
    }

    /* Count power of 7 */
    while (tmp % 7ULL == 0) {
        tmp /= 7ULL;
    }

    /* Factor odd primes >= 3 */
    for (uint64_t p = 3ULL; p <= tmp / p; p += 2ULL) {
        if (tmp % p == 0) {
            uint64_t exp = 0ULL;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            int ls = legendre7(p);   /* (7|p) */
            if (ls == 1) {
                /* a(p^e) = e+1 */
                res *= (seq_t)(exp + 1);
            } else if (ls == -1) {
                /* a(p^e) = 0 if e is odd, else 1 */
                if (exp & 1ULL)
                    return (seq_t)0;
                /* even exponent gives factor 1 → nothing to do */
            }
        }
    }

    /* If any prime factor remains larger than sqrt(original n) */
    if (tmp > 1ULL) {
        uint64_t p = tmp;
        int ls = legendre7(p);   /* p is prime, p ≠ 2,7 by construction */
        if (ls == 1) {
            /* exponent is 1, so a(p) = 2 */
            res *= (seq_t)2;
        } else if (ls == -1) {
            /* odd exponent (1) → zero result */
            return (seq_t)0;
        }
    }

    return res;
}
