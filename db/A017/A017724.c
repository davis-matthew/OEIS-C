/*
 * A017724  Binomial coefficients C(n,60).
 * https://oeis.org/A017724
 *
 * Offset:    60    (a(60) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,60)
 * Range:     u8 n <= 61, u16 n <= 63, u32 n <= 67, u64 n <= 81, u128 n <= 132,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 60ul)
        return 0;           /* undefined below the offset */

    /* num[i] = n-59+i   (i = 0..59)   */
    /* den[i] = i+1      (i = 0..59)   */
    seq_t num[60];
    seq_t den[60];
    for (int i = 0; i < 60; ++i) {
        num[i] = (seq_t)(n - 59 + i);
        den[i] = (seq_t)(i + 1);
    }

    /* Reduce numerator and denominator by gcds */
    for (int i = 0; i < 60; ++i) {
        for (int j = 0; j < 60; ++j) {
            if (den[j] == 1u)
                continue;
            seq_t g = gcd(num[i], den[j]);
            if (g > 1u) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    /* Multiply the remaining numerator terms */
    seq_t result = 1;
    for (int i = 0; i < 60; ++i)
        result *= num[i];

    return result;
}
