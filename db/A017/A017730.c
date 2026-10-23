/*
 * A017730  Binomial coefficients C(n,66).
 * https://oeis.org/A017730
 *
 * Offset:    66    (a(66) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,66)
 * Range:     u8 n <= 67, u16 n <= 69, u32 n <= 73, u64 n <= 86, u128 n <= 131,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* The OEIS offset for this sequence is 66.
       For argument values below the offset the behaviour is
       unspecified; we return 0 for convenience. */
    if (n < 66) {
        return 0;
    }

    /* Compute C(n,66) multiplicatively by cancelling
       common factors between the numerator and the
       denominator before any multiplication.
       This keeps intermediate values as small as
       possible and works for all seq_t sizes. */
    const idx_t k = 66;
    seq_t num[66];
    seq_t den[66];

    /* Initialise numerator and denominator factors. */
    idx_t i;
    for (i = 0; i < k; ++i) {
        num[i] = (seq_t)(n - k + i + 1);  /* n-65, n-64, …, n          */
        den[i] = (seq_t)(i + 1);          /* 1, 2, …, 66               */
    }

    /* Reduce all numerators against all denominators. */
    for (i = 0; i < k; ++i) {
        idx_t j;
        for (j = 0; j < k; ++j) {
            seq_t g = gcd(num[i], den[j]);
            if (g > 1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    /* The remaining denominators should all be 1; multiply
       all numerator factors to obtain the binomial coefficient. */
    seq_t result = 1;
    for (i = 0; i < k; ++i) {
        result *= num[i];
    }

    return result;
}
