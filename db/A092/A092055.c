/*
 * A092055  a(n) = binomial(2 + 2^n,3).
 * https://oeis.org/A092055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2 + 2^n, 3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   mul_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_pow2.h"

seq_t a(idx_t n) {
    seq_t k = mul_pow2(n);  /* k = 2^n       */
    k += 2;                 /* k = 2 + 2^n   */

    /* Values for the three consecutive integers k, k-1, k-2. */
    seq_t vals[3] = {k, k - 1, k - 2};

    /* Divide out factors 2 and 3 to avoid overflow. */
    const unsigned int divisors[2] = {2U, 3U};
    for (unsigned int d_idx = 0; d_idx < 2; ++d_idx) {
        unsigned int d = divisors[d_idx];
        for (int i = 0; i < 3; ++i) {
            if (vals[i] % d == 0) {
                vals[i] /= d;
                break;
            }
        }
    }

    /* Multiply the remaining factors to get the binomial coefficient. */
    seq_t result = vals[0];
    result *= vals[1];
    result *= vals[2];
    return result;
}
