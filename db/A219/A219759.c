/*
 * A219759  Expansion of
 *          x^4*(2-12*x+24*x^2-8*x^3-41*x^4+57*x^5-16*x^6)/((1-x)*(1-3*x+x^2)*(1-2*x)^6).
 * https://oeis.org/A219759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. =
 *            x^4*(2-12*x+24*x^2-8*x^3-41*x^4+57*x^5-16*x^6)/((1-x)*(1-3*x+x^2)*(1-2*x)^6)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* recurrence coefficients derived from the denominator */
static const int32_t pos_lags[4] = {2, 4, 6, 8};
static const int32_t pos_coefs[4] = {112, 1132, 1952, 448};

static const int32_t neg_lags[5] = {1, 3, 5, 7, 9};
static const int32_t neg_coefs[5] = {16, 449, 1852, 1264, 64};

/* numerator coefficients for n = 0 .. 10 (only up to degree 10) */
static const int32_t num_coefs[11] = {
    0,      /* n = 0 */
    0,      /* n = 1 */
    0,      /* n = 2 */
    0,      /* n = 3 */
    2,      /* n = 4 */
    -12,    /* n = 5 */
    24,     /* n = 6 */
    -8,     /* n = 7 */
    -41,    /* n = 8 */
    57,     /* n = 9 */
    -16     /* n =10 */
};

/* sequence value a(n) */
seq_t a(idx_t n)
{
    /* allocate array of size n+1 */
    size_t N = (size_t)n + 1;
    seq_t *seq = (seq_t*) calloc(N, sizeof(seq_t));
    if (!seq) return 0;            /* safety */

    /* iterate and compute values */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* positive coefficients */
        for (int i = 0; i < 4; ++i) {
            int lag = pos_lags[i];
            if ((idx_t)lag <= k) {
                seq_t term = seq[k - lag];
                sum_pos += (seq_t)(pos_coefs[i]) * term;
            }
        }

        /* negative coefficients */
        for (int i = 0; i < 5; ++i) {
            int lag = neg_lags[i];
            if ((idx_t)lag <= k) {
                seq_t term = seq[k - lag];
                sum_neg += (seq_t)(neg_coefs[i]) * term;
            }
        }

        /* apply numerator correction */
        int32_t p = (k <= 10) ? num_coefs[k] : 0;
        seq_t temp = sum_neg;
        if (p >= 0) {
            temp += (seq_t)p;
        } else {
            temp -= (seq_t)(-p);
        }

        /* final value a[k] = temp - sum_pos */
        seq[k] = temp - sum_pos;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
