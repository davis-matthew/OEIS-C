/*
 * A220665  Array of coefficients of powers of x^2 for (S(2*n+1,x)/x)^3, with
 *          Chebyshev's S polynomials A049310.
 * https://oeis.org/A220665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) = coefficient of x^(2k) in (S(2r+1,x)/x)^3 * where r =
 *            largest integer with r*(3r-1)/2 ≤ n (triangular index) * and k =
 *            n - r*(3r-1)/2, 0 ≤ k ≤ 3r. S(m,x) are Chebyshev *
 *            S-polynomials: S(m,x) = ∑_{j=0}^{⌊m/2⌋} (-1)^j * binom(m-j,j) *
 *            x^(m-2j)
 * Range:     i8 n <= 6, i16 n <= 25, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, free */
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */

static inline idx_t cumulative_terms(idx_t r) {
    /* S(r) = r*(3r-1)/2 */
    __int128 tmp = (__int128)r * (3*(__int128)r - 1) / 2;
    return (idx_t)tmp;
}

static idx_t find_row(idx_t n) {
    /* binary search for largest r with cumulative_terms(r) ≤ n */
    idx_t lo = 0, hi = 1;
    while (cumulative_terms(hi) <= n) {
        hi <<= 1;                       /* exponential increase */
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cumulative_terms(mid) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Determine row r and offset k */
    idx_t r = find_row(n);
    idx_t k = n - cumulative_terms(r);          /* 0 ≤ k ≤ 3r */
    idx_t exp = 2 * k;                           /* exponent of x */

    /* Allocate coefficient arrays for P, P² and P³ */
    idx_t lenP  = 2 * r + 1;                     /* degrees 0..2r */
    idx_t lenP2 = 4 * r + 1;                     /* degrees 0..4r */
    idx_t lenP3 = 6 * r + 1;                     /* degrees 0..6r */

    seq_t *p  = (seq_t *)malloc(lenP  * sizeof(seq_t));
    seq_t *p2 = (seq_t *)malloc(lenP2 * sizeof(seq_t));
    seq_t *p3 = (seq_t *)malloc(lenP3 * sizeof(seq_t));

    if (!p || !p2 || !p3) {
        /* Allocation failure – return 0 as safe fallback */
        free(p); free(p2); free(p3);
        return 0;
    }

    /* Initialize arrays to zero */
    for (idx_t i = 0; i < lenP;  ++i) p[i]  = 0;
    for (idx_t i = 0; i < lenP2; ++i) p2[i] = 0;
    for (idx_t i = 0; i < lenP3; ++i) p3[i] = 0;

    /* Build P(x) = S(2r+1,x)/x */
    for (idx_t s = 0; s <= r; ++s) {
        idx_t idx = 2 * s;                    /* even exponent */
        seq_t coeff = binom(r + 1 + s, r - s);/* binomial part   */
        if ((r - s) & 1)                     /* sign adjustment  */
            coeff = -coeff;
        p[idx] = coeff;
    }

    /* Compute P² = P * P */
    for (idx_t i = 0; i <= 2 * r; ++i) {
        if (p[i] == 0) continue;             /* skip zero entries */
        for (idx_t j = 0; j <= 2 * r; ++j) {
            if (p[j] == 0) continue;
            p2[i + j] += p[i] * p[j];
        }
    }

    /* Compute P³ = P² * P */
    for (idx_t i = 0; i <= 4 * r; ++i) {
        if (p2[i] == 0) continue;
        for (idx_t j = 0; j <= 2 * r; ++j) {
            if (p[j] == 0) continue;
            p3[i + j] += p2[i] * p[j];
        }
    }

    /* Desired coefficient corresponds to exponent 2k = exp */
    seq_t result = p3[exp];

    /* Clean up */
    free(p);
    free(p2);
    free(p3);

    return result;
}
