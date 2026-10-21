/*
 * A228913  a(n) =
 *          11^n-10*10^n+45*9^n-120*8^n+210*7^n-252*6^n+210*5^n-120*4^n+45*3^n-10*2^n+1.
 * https://oeis.org/A228913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            11^n-10*10^n+45*9^n-120*8^n+210*7^n-252*6^n+210*5^n-120*4^n+45*3^n-10*2^n+1
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Coefficients for bases 11 down to 1. Positive first, negative later. */
    static const int coeffs[11] = {
        1, -10, 45, -120, 210, -252, 210, -120, 45, -10, 1
    };

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 0; i < 11; ++i) {
        /* Base values are 11-i: 11,10,...,1 */
        seq_t pow_val = pow_uint((uint64_t)(11 - i), n);
        int c = coeffs[i];

        if (c > 0) {
            sum_pos += pow_val * (seq_t)c;
        } else {
            sum_neg += pow_val * (seq_t)(-c);
        }
    }

    return sum_pos - sum_neg;
}
