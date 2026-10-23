/*
 * A133889  a(n) = binomial(n+9,n) mod 9.
 * https://oeis.org/A133889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9) mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Computes gcd for uint64_t values */

seq_t a(idx_t n) {
    /* Compute binomial(n+9,9) by cancelling factors of the denominator
       and then taking the result modulo 9. */
    uint64_t num[9];
    for (uint64_t i = 0; i < 9; ++i) {
        num[i] = n + (i + 1);
    }

    /* Cancel denominator factors 2..9 */
    for (uint64_t d = 2; d <= 9; ++d) {
        uint64_t denom = d;
        for (uint64_t j = 0; j < 9 && denom > 1; ++j) {
            uint64_t g = gcd_u64(num[j], denom);
            if (g > 1) {
                num[j] /= g;
                denom /= g;
            }
        }
    }

    /* Multiply remaining numerator factors modulo 9 */
    uint64_t prod_mod9 = 1;
    for (uint64_t i = 0; i < 9; ++i) {
        prod_mod9 = (prod_mod9 * (num[i] % 9)) % 9;
    }

    return (seq_t)prod_mod9;
}
