/*
 * A213287  Number of 8-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; for n ≥ 1: a(n) = Σ_{k=0..7} c[k]·binom(n+7−k, 8)
 *            where c = {1, 82, 86, 266, 1273, 4234, 5880, 28498}. The g.f. is
 *            x·(1+82x+86x²+266x³+1273x⁴+4234x⁵+5880x⁶+28498x⁷)/(1−x)⁹
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 258, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Special case for the empty alphabet */
    if (n == 0)
        return 0;

    /* Coefficients from the numerator of the generating function */
    static const seq_t coeff[8] = {
        (seq_t)1ULL,
        (seq_t)82ULL,
        (seq_t)86ULL,
        (seq_t)266ULL,
        (seq_t)1273ULL,
        (seq_t)4234ULL,
        (seq_t)5880ULL,
        (seq_t)28498ULL
    };

    seq_t result = 0;
    for (idx_t k = 0; k < 8; ++k) {
        /* a + b + ...: compute the binomial coefficient binom(n+7−k, 8) */
        idx_t arg = n + 7 - k;
        if (arg < 8)   /* binom(arg,8) would be zero */
            continue;
        result += coeff[k] * binom(arg, 8);
    }
    return result;
}
