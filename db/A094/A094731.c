/*
 * A094731  Number of connected ordered 4-element multiantichains on a labeled
 *          n-set.
 * https://oeis.org/A094731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1·15ⁿ – 12·11ⁿ + 24·9ⁿ – 8·7ⁿ + 27·6ⁿ – 96·5ⁿ – 6·4ⁿ +
 *            246·3ⁿ – 288·2ⁿ + 138·1ⁿ (the constant –26·0ⁿ contributes only
 *            when n = 0, giving a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static const int coeffs[10] = {  1, -12,  24,  -8,  27, -96,  -6,
                                    246, -288, 138 };
    static const int bases[10]  = { 15, 11,   9,   7,   6,  5,   4,
                                   3,   2,    1 };

    seq_t pos = 0;   /* sum of positive terms */
    seq_t neg = 0;   /* sum of terms with negative coefficient */

    for (int i = 0; i < 10; ++i) {
        /* compute bases[i]^n */
        seq_t pow_val = big_pow((seq_t)bases[i], (seq_t)n);

        /* |coeff| */
        seq_t coeff_abs = (seq_t)(coeffs[i] > 0 ? coeffs[i] : -coeffs[i]);

        /* term = |coeff| * base^n */
        seq_t term = pow_val * coeff_abs;

        if (coeffs[i] > 0)
            pos += term;
        else
            neg += term;
    }

    /* a(n) = pos – neg  (positively defined for every n ≥ 1) */
    return pos - neg;
}
