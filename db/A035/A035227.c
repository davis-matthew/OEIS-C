/*
 * A035227  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          45.
 * https://oeis.org/A035227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(45, d) The implementation uses the
 *            multiplicative rule: for a prime power p^e, a(p^e) = 1 if p = 3
 *            or 5 = 1 if (5/p) = -1 and e even = 0 if (5/p) = -1 and e odd =
 *            e+1 if (5/p) = +1 Products over the prime-factorization of n
 *            yield a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t x = (uint64_t)n;

    /* handle factor 2 */
    if ((x & 1ULL) == 0) {
        uint64_t e = 0;
        while ((x & 1ULL) == 0) { x >>= 1; ++e; }
        seq_t factor = (e & 1) ? 0 : 1;   /* (5/2) = -1 */
        result *= factor;
        if (result == 0) return 0;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; ++e; }

            seq_t factor;
            if (p == 3 || p == 5) {
                factor = 1;
            } else {
                uint64_t r = p % 5;
                if (r == 1 || r == 4) {            /* (5/p) = +1 */
                    factor = (seq_t)(e + 1);
                } else { /* r == 2 or 3: (5/p) = -1 */
                    factor = (e & 1) ? 0 : 1;
                }
            }
            result *= factor;
            if (result == 0) return 0;
        }
    }

    /* leftover prime factor */
    if (x > 1) {
        uint64_t p = x;
        seq_t factor;
        if (p == 3 || p == 5) {
            factor = 1;
        } else {
            uint64_t r = p % 5;
            if (r == 1 || r == 4) {            /* (5/p) = +1, e = 1 */
                factor = 2;
            } else { /* (5/p) = -1, e = 1 */
                factor = 0;
            }
        }
        result *= factor;
    }

    return result;
}
