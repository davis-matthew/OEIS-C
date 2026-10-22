/*
 * A005784  Number of 4-covers of an unlabeled n-set.
 * https://oeis.org/A005784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   G.f.: (x^20 - x^19 + 4*x^18 + 9*x^17 + 23*x^16 + 39*x^15 +
 *            90*x^14 + 131*x^13 + 204*x^12 + 238*x^11 + 252*x^10 + 238*x^9 +
 *            204*x^8 + 131*x^7 + 90*x^6 + 39*x^5 + 23*x^4 + 9*x^3 + 4*x^2 - x
 *            + 1)/((1 - x^4)^3*(1 - x^3)^4*(1 - x^2)^3*(1 - x)^5); computed
 *            by expanding the series: the numerator divided...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 29, u64 n <= 172, u128 n <= 4281,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const long long num_coef[21] = {
    1, -1, 4, 9, 23, 39, 90, 131, 204, 238, 252,
    238, 204, 131, 90, 39, 23, 9, 4, -1, 1
};

seq_t a(idx_t n)
{
    long long pre[21];
    long long s = 0;
    idx_t i, len = n + 1;
    int f, k, rep;
    seq_t *b;
    seq_t r;
    static const int strides[3] = {2, 3, 4};
    static const int reps[3] = {3, 4, 3};

    for (k = 0; k < 21; k++) {
        s += num_coef[k];
        pre[k] = s;
    }

    b = (seq_t *)malloc(sizeof(seq_t) * (size_t)len);
    for (i = 0; i < len; i++)
        b[i] = (seq_t)(i < 20 ? pre[i] : pre[20]);

    /* remaining 4 factors of 1/(1-x) */
    for (rep = 0; rep < 4; rep++)
        for (i = 1; i < len; i++)
            b[i] += b[i - 1];

    for (f = 0; f < 3; f++) {
        idx_t st = (idx_t)strides[f];
        for (rep = 0; rep < reps[f]; rep++)
            for (i = st; i < len; i++)
                b[i] += b[i - st];
    }

    r = b[n];
    free(b);
    return r;
}
