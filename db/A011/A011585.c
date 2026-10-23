/*
 * A011585  Legendre symbol (n,19).
 * https://oeis.org/A011585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Legendre symbol (n,19)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   legendre_mod19_unit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre_mod19_unit.h"

seq_t a(idx_t n) {
    /* Handle the offset (a(0)=0) and the zero at multiples of 19. */
    if (n == 0U) return (seq_t)0;
    if (n % 19U == 0U) return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t tmp   = (uint64_t)n;

    /* Factor via trial division. */
    for (uint64_t p = 2U; p <= tmp / p; ++p) {
        if (tmp % p == 0U) {
            unsigned cnt = 0U;
            while (tmp % p == 0U) {
                ++cnt;
                tmp /= p;
            }
            if (cnt & 1U) {               /* odd exponent contributes */
                seq_t leg = legendre_mod19_unit(p);
                if (leg == (seq_t)0) return (seq_t)0;
                result *= leg;
            }
        }
    }
    /* Remaining large prime factor (if any). */
    if (tmp > 1U) {
        seq_t leg = legendre_mod19_unit(tmp);
        if (leg == (seq_t)0) return (seq_t)0;
        result *= leg;
    }

    return result;
}
