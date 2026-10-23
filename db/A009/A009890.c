/*
 * A009890  Coordination sequence T1 for Zeolite Code RTE.
 * https://oeis.org/A009890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 174, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the OEIS sequence A009890 (Coordination sequence T1 for Zeolite Code RTE)
 *
 * The sequence is defined by the rational generating function
 *
 *   G(x) = N(x) / D(x)
 *
 * where
 *
 * D(x) = (1 - x)^3 * (1 + x^2) * (1 + x + x^2) *
 *        (1 + x + x^2 + x^3 + x^4) * (1 + x + x^2 + x^3 + x^4 + x^5 + x^6)
 *
 * and
 *
 * N(x) = (1 + x) * (1 + 3x + 8x^2 + 14x^3 + 25x^4 + 38x^5 + 49x^6 + 57x^7 +
 *         59x^8 + 57x^9 + 51x^10 + 39x^11 + 26x^12 + 15x^13 + 7x^14 + x^15 -
 *         x^17 + x^18 + x^19).
 *
 * From the generating function one obtains a linear homogeneous recurrence of
 * order 17 with constant coefficients (the coefficients of D(x)).
 * The first 17 terms are taken from the OEIS b-file as initial values.
 */
#include <stdlib.h>

/* Initial 17 terms (a(0) ... a(16)) */
static const seq_t init[17] = {
    1, 4, 10, 19, 33, 56, 81, 105,
    136, 175, 219, 262, 307, 363, 425, 485, 548
};

/* Coefficients d[1] .. d[17] of the denominator polynomial D(x)
 * D(x) = 1 + 0*x + 1*x^2 - 1*x^3 + 0*x^4 - 2*x^5 + 0*x^6 - 2*x^7 +
 *        1*x^8 - 1*x^9 + 2*x^10 + 0*x^11 + 2*x^12 + 0*x^13 + 1*x^14 -
 *        1*x^15 + 0*x^16 - 1*x^17
 */
static const int d[18] = {
    0,  /* dummy for d[0] */
    0,  1, -1, 0, -2, 0, -2, 1, -1, 2, 0, 2, 0, 1, -1, 0, -1
};

/* Coefficients of the numerator polynomial N(x) for indices 0 .. 20.
 * For indices > 20 the coefficients are zero.
 */
static const int ncoeff[21] = {
    1,  4,  11, 22, 39, 63, 87, 106,
    116, 116, 108, 90, 65, 41, 22, 8, 1, -1, 0, 2, 1
};

seq_t a(idx_t n)
{
    /* Trivial case: use the pre‑computed initial values. */
    if (n < 17) {
        return init[n];
    }

    /* Allocate an array large enough to store all terms up to n. */
    size_t len = (size_t)n + 1;
    seq_t *vals = (seq_t *)malloc(len * sizeof(seq_t));
    if (!vals) {
        /* Caller guarantees that n is modest; aborting is acceptable here. */
        abort();
    }

    /* Initialise the known first 17 terms. */
    for (idx_t i = 0; i < 17; ++i) {
        vals[i] = init[i];
    }

    /* Compute the remaining terms iteratively using the recurrence. */
    for (idx_t m = 17; m < len; ++m) {
        long long sum = 0;                      /* use signed 64‑bit sum */
        for (int j = 1; j <= 17; ++j) {
            sum += (long long)d[j] * (long long)vals[m - j];
        }
        long long val = -sum;                   /* homogeneous part */
        if (m <= 20) {
            val += ncoeff[m];                    /* add numerator coefficient when present */
        }
        vals[m] = (seq_t)val;
    }

    seq_t result = vals[n];
    free(vals);
    return result;
}
