/*
 * A138094  A triangular sequence of eight back recursive polynomials that are
 *          Hermite H(x,n) like and alternating orthogonal on domain
 *          {-Infinity,Infinity} and weight function Exp[ -x^2/2]:k=8 P(x, n)
 *          = Sum[If[Mod[m, 2] == 1, (m + 1)*x^m*P(x, n - m), n^(m/2)*P(x, n -
 *          m)], {m, 1, k}].
 * https://oeis.org/A138094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   a(n) is the (n - r(r‑1)/2)-th coefficient of the polynomial *
 *            P_r(x) defined by the back‑recursive rule (k = 8) * * For n >=
 *            1: * P_1(x) = 1 * For i > 1: * P_i(x) = Σ_{m=1..8} T(i,m) *
 *            where * if m is odd: T(i,m) = (m+1) * x^m * P_{i-m}(x) * if m is
 *            even: T(i,m) = (i-1)^(m/2) * P_{i-m}(x) * All P...
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define K 8

/* power with small integer exponent (exponent <= 4) */
static inline seq_t pow_small_uint(seq_t base, int exp) {
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

/* Main sequence generator */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n=0, but return 0 */

    /* --------- determine the row r that contains index n ----------- */
    idx_t r = 1;
    /* use 128‑bit to avoid overflow when computing r*(r+1)/2 */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* column within the row (1‑based) */
    idx_t col = n - (r * (r - 1)) / 2;  /* r*(r-1)/2 + 1 is the first index of row r */
    /* convert to 0‑based index into the coefficient array */
    idx_t idx = col - 1;

    /* --------- allocate storage for all rows up to r --------------- */
    seq_t **coef = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
    if (!coef) return (seq_t)0;  /* allocation failure */

    coef[0] = NULL;  /* unused */
    for (idx_t i = 1; i <= r; ++i) {
        coef[i] = (seq_t*)calloc((size_t)i, sizeof(seq_t));
        if (!coef[i]) { /* cleanup */
            for (idx_t j = 1; j < i; ++j) free(coef[j]);
            free(coef);
            return (seq_t)0;
        }

        /* base polynomial P_1(x) = 1 */
        if (i == 1) {
            coef[1][0] = 1;
            continue;
        }

        /* compute P_i(x) from lower rows */
        for (int m = 1; m <= K && m <= i; ++m) {
            idx_t prev = i - m;          /* index of preceding polynomial */
            if (prev == 0) continue;     /* P_0 and below are zero */

            seq_t *src = coef[prev];
            int prev_len = (int)prev;
            seq_t scale;
            int shift = 0;

            if (m & 1) {                 /* m odd */
                scale = (seq_t)(m + 1);
                shift = m;
            } else {                     /* m even */
                scale = pow_small_uint((seq_t)(i - 1), m / 2);
                shift = 0;
            }

            for (int j = 0; j < prev_len; ++j) {
                seq_t add = src[j] * scale;
                coef[i][j + shift] += add;
            }
        }
    }

    seq_t result = coef[r][idx];

    /* cleanup */
    for (idx_t i = 1; i <= r; ++i) free(coef[i]);
    free(coef);

    return result;
}
