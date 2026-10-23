/*
 * A022258  Gaussian binomial coefficients [ n,7 ] for q = 9.
 * https://oeis.org/A022258
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PROD_{i=1..7} (9^(n-i+1)-1)/(9^i-1)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* values for n < 7 are outside the offset; return 0 */
    if (n < 7) return 0;

    const int k = 7;
    seq_t numerators[k];
    seq_t denominators[k];
    seq_t one = 1;

    /* compute the numerator and denominator factors */
    for (int i = 1; i <= k; ++i) {
        numerators[i - 1] = big_pow((seq_t)9, (seq_t)(n - i + 1)) - 1;
        denominators[i - 1] = big_pow((seq_t)9, (seq_t)i) - 1;
    }

    /* cancel common factors with the denominators */
    for (int i = 0; i < k; ++i) {
        seq_t denom = denominators[i];
        if (denom == one) continue;
        for (int j = 0; j < k; ++j) {
            seq_t num = numerators[j];
            seq_t g = gcd(num, denom);
            if (g == one) continue;
            numerators[j] = num / g;
            denom = denom / g;
            if (denom == one) break;
        }
    }

    /* multiply the remaining numerators to obtain the result */
    seq_t result = one;
    for (int j = 0; j < k; ++j) {
        result = result * numerators[j];
    }

    return result;
}
