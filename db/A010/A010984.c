/*
 * A010984  Binomial coefficient C(n,31).
 * https://oeis.org/A010984
 *
 * Offset:    31    (a(31) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, 31) (C(n,31))
 * Range:     u8 n <= 32, u16 n <= 35, u32 n <= 42, u64 n <= 67, u128 n <= 232,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 31) return 0;          /* undefined below offset */

    const idx_t k = 31;
    seq_t numer[31];
    seq_t denom[31];

    /* Fill numerator and denominator factors  */
    for (idx_t i = 0; i < k; ++i) {
        numer[i] = (seq_t)(n - k + 1 + i);   /* n-k+1 .. n   */
        denom[i] = (seq_t)(i + 1);           /* 1 .. k       */
    }

    /* Reduce common factors between numerators and denominators */
    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = 0; j < k; ++j) {
            seq_t g = gcd(numer[i], denom[j]);
            if (g > 1) {
                numer[i] /= g;
                denom[j] /= g;
                if (denom[j] == 1) continue;
            }
        }
    }

    /* Multiply the remaining numerator factors */
    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        result *= numer[i];
    }

    return result;
}
