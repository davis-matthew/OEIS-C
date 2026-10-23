/*
 * A015394  Gaussian binomial coefficient [ n,10 ] for q=-8.
 * https://oeis.org/A015394
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..10} ((-8)^(n-i+1)-1)/((-8)^i-1)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 10 (offset 10). */
    if (n < 10) return 0;   /* behaviour below the offset is irrelevant */

    /* Absolute values of the binomial factors. */
    seq_t abs_num[10];
    seq_t abs_den[10];

    /* Numerators:
       for i = 1..10 we need: |(-8)^(n-i+1) - 1|   */
    for (idx_t i = 1; i <= 10; ++i) {
        idx_t exp_num = n - i + 1;           /* exponent in numerator */
        seq_t pow_val = big_pow((seq_t)8, (seq_t)exp_num);

        if (exp_num & 1U) { /* odd exponent -> -(8^e) - 1 = -(8^e+1) */
            abs_num[i - 1] = pow_val + 1;
        } else {            /* even exponent -> 8^e - 1 */
            abs_num[i - 1] = pow_val - 1;
        }
    }

    /* Denominators:
       for i = 1..10 we need: |(-8)^i - 1|   */
    for (idx_t i = 1; i <= 10; ++i) {
        idx_t exp_den = i;
        seq_t pow_val = big_pow((seq_t)8, (seq_t)exp_den);

        if (exp_den & 1U) { /* odd exponent -> -(8^i) - 1 = -(8^i+1) */
            abs_den[i - 1] = pow_val + 1;
        } else {            /* even exponent -> 8^i - 1 */
            abs_den[i - 1] = pow_val - 1;
        }
    }

    /* Cancel denominator factors against numerator factors. */
    for (int j = 0; j < 10; ++j) {
        seq_t d = abs_den[j];
        for (int i = 0; i < 10 && d > 1; ++i) {
            seq_t g = gcd(abs_num[i], d);
            if (g > 1) {
                abs_num[i] /= g;
                d /= g;
            }
        }
        /* After this loop d should be 1 because the binomial
           coefficient is an integer. */
    }

    /* Multiply the remaining numerator factors to obtain the result. */
    seq_t result = 1;
    for (int i = 0; i < 10; ++i) {
        result *= abs_num[i];
    }

    return result;
}
