/*
 * A083024  Molien series for action of SL(3,C) on ternary forms of degree 4.
 * https://oeis.org/A083024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of z^(3n) in
 * (1 + z^9 + z^12 + z^15 + 2*z^18 + 3*z^21 + 2*z^24 + 3*z^27 + 4*z^30 +
 *  3*z^33 + 4*z^36 + 4*z^39 + 3*z^42 + 4*z^45 + 3*z^48 + 2*z^51 + 3*z^54 +
 *  2*z^57 + z^60 + z^63 + z^75) /
 * ((1 - z^3)(1 - z^6)(1 - z^9)(1 - z^12)(1 - z^15)(1 - z^18)(1 - z^27))
 */
#include <stdlib.h>

/* compute coefficient of z^(3n) in the above rational function */
seq_t a(idx_t n)
{
    /* part sizes in the denominator: 3,6,9,12,15,18,27 */
    static const idx_t K[]   = {3, 6, 9, 12, 15, 18, 27};
    const unsigned Klen = sizeof K / sizeof *K;

    /* numerator polynomial degrees and coefficients */
    static const idx_t degs[]   = {0, 9, 12, 15, 18, 21, 24, 27, 30, 33,
                                   36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 75};
    static const seq_t coeffs[] = {1, 1, 1, 1, 2, 3, 2, 3, 4, 3,
                                   4, 4, 3, 4, 3, 2, 3, 2, 1, 1, 1};
    const unsigned Ndeg = sizeof degs / sizeof *degs;   /* 21 */

    const idx_t maxDegInN = 75;
    /* maximum exponent we need to consider */
    idx_t needM = 3 * n + maxDegInN;

    /* allocate array for partition numbers c[0 .. needM] */
    seq_t *c = (seq_t *)calloc(needM + 1, sizeof(seq_t));
    if (!c)
        return 0; /* allocation failure (unlikely) */

    c[0] = 1;
    /* recurrence for partitions with allowed parts K */
    for (unsigned ki = 0; ki < Klen; ++ki) {
        idx_t k = K[ki];
        for (idx_t m = k; m <= needM; ++m) {
            c[m] += c[m - k];
        }
    }

    /* compute a(n) from numerator polynomial */
    seq_t result = 0;
    idx_t zpower = 3 * n;
    for (unsigned i = 0; i < Ndeg; ++i) {
        if (zpower >= degs[i]) {
            result += coeffs[i] * c[zpower - degs[i]];
        }
    }

    free(c);
    return result;
}
