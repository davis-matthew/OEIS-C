/*
 * A017750  Binomial coefficients C(n,86).
 * https://oeis.org/A017750
 *
 * Offset:    86    (a(86) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,86) – the binomial coefficient “n choose 86”. The
 *            sequence is indexed so that a(86)=1, a(87)=87, a(88)=3828, …
 * Range:     u8 n <= 87, u16 n <= 88, u32 n <= 92, u64 n <= 103,
 *            u128 n <= 137, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n) {
    /* The sequence is defined only for n >= 86.
       Indices below the offset may return 0 – they are ignored by the tests. */
    if (n < 86)
        return 0;

    /* Compute C(n,86) multiplicatively:
       result = 1
       for k = 1..86:
           numerator   = n - 86 + k
           denominator = k
           cancel common factors between result and denominator,
           then cancel remaining common factors between numerator and denominator,
           finally multiply result by the now-coprime numerator.              */
    seq_t result = 1;

    for (idx_t k = 1; k <= 86; ++k) {
        seq_t numerator   = (seq_t)(n - 86 + k);
        seq_t denominator = (seq_t)k;

        /* Cancel common factors with the current result */
        seq_t g1 = seq_gcd(result, denominator);
        result /= g1;
        denominator /= g1;

        /* Cancel remaining common factors with the numerator */
        seq_t g2 = seq_gcd(numerator, denominator);
        numerator /= g2;
        denominator /= g2;

        /* After the cancellations denominator should be 1
           (the division performed by the binomial formula is exact). */
        result = result * numerator;
        /* No further division needed. */
    }

    return result;
}
