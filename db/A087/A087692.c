/*
 * A087692  Number of cubes in multiplicative group modulo n.
 * https://oeis.org/A087692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = φ(n) / 3ᵁ where U = #{pᵏ‖n | p≡1 (mod 6) or p = 3,k ≥ 2}
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 */
        return (seq_t)0;

    idx_t m = n;
    seq_t phi = (seq_t)n;         /* φ(n) starts as n */
    idx_t t = 0;                  /* exponent of 3 in denominator */

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* update φ(n) */
            phi /= (seq_t)p;
            phi *= (seq_t)(p - 1);

            /* count whether to divide φ by 3 for this prime power */
            if (p == 3) {
                if (e >= 2)
                    ++t;
            } else if (p % 6 == 1) {
                ++t;
            }
        }
    }

    /* last prime factor, if any */
    if (m > 1) {
        /* m is prime */
        phi /= (seq_t)m;
        phi *= (seq_t)(m - 1);

        if (m == 3) {
            /* exponent is 1, does not contribute */
        } else if (m % 6 == 1) {
            ++t;
        }
    }

    /* compute 3ᵀ as seq_t */
    seq_t denom = 1;
    for (idx_t i = 0; i < t; ++i)
        denom *= (seq_t)3;

    return phi / denom;
}
