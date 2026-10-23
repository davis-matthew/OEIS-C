/*
 * A138093  A triangular sequence of six back recursive polynomial that are
 *          Hermite H(x,n) like and alternating orthogonal on domain
 *          {-Infinity,Infinity} and weight function Exp[ -x^2/2]:k=6 P(x, n)
 *          = Sum[If[Mod[m, 2] == 1, (m + 1)*x^m*P(x, n - m), n^(m/2)*P(x, n -
 *          m)], {m, 1, k}].
 * https://oeis.org/A138093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   a(n) = the n‑th coefficient when the coefficients of the *
 *            back‑recursive polynomial P(x,n) (k = 6) are listed row by row *
 *            * P(x,0) = 0 * P(x,1) = 1 * P(x,n) = Σ_{m=1..6} * If m odd:
 *            (m+1) * x^m * P(x, n-m) * If m even: (n-1)^(m/2) * P(x, n-m) * *
 *            The flattened list of all coefficients gives A13...
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper to construct a row of seq_t values when compiled as C++.
 * In C this code is ineffective and harmless. */
#ifdef __cplusplus
#include <new>
static void seq_t_default_construct(seq_t *p, size_t len) {
    for (size_t i = 0; i < len; ++i) new(&p[i]) seq_t();
}
static void seq_t_destruct(seq_t *p, size_t len) {
    for (size_t i = 0; i < len; ++i) p[i].~seq_t();
}
#else
static void seq_t_default_construct(seq_t *p, size_t len) { (void)p; (void)len; }
static void seq_t_destruct(seq_t *p, size_t len)   { (void)p; (void)len; }
#endif

/* Compute the triangular sequence A138093. */
seq_t a(idx_t n)
{
    if (n < 1U) return 0;                /* out of defined range */

    /* Find the row r that contains the requested coefficient. */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n )
        ++r;

    /* Position of the coefficient inside row r (0‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t idx_within_row = n - prev - 1;   /* 0‑based index in row r */

    /* Allocate an array of pointers for rows 0..r. */
    seq_t **rows = (seq_t **)malloc((size_t)(r + 1) * sizeof(seq_t *));
    if (!rows) return 0;                  /* allocation failure */

    /* Row 0: single coefficient equal to 0. */
    rows[0] = (seq_t *)malloc(sizeof(seq_t));
    if (rows[0]) {
        seq_t_default_construct(rows[0], 1);
        rows[0][0] = 0;
    } else {
        free(rows);
        return 0;
    }

    /* Row 1: single coefficient equal to 1. */
    rows[1] = (seq_t *)malloc(sizeof(seq_t));
    if (rows[1]) {
        seq_t_default_construct(rows[1], 1);
        rows[1][0] = 1;
    } else {
        free(rows[0]); free(rows);
        return 0;
    }

    /* Compute rows 2 .. r. */
    for (idx_t nrow = 2; nrow <= r; ++nrow) {
        /* Allocate space for nrow coefficients. */
        rows[nrow] = (seq_t *)malloc((size_t)nrow * sizeof(seq_t));
        if (!rows[nrow]) {
            /* clean up on failure */
            for (idx_t k = 0; k <= nrow; ++k) {
                if (rows[k]) {
                    seq_t_destruct(rows[k], k);
                    free(rows[k]);
                }
            }
            free(rows);
            return 0;
        }

        /* Default construct elements if needed. */
        seq_t_default_construct(rows[nrow], nrow);

        /* Initialize to zero. */
        for (idx_t j = 0; j < nrow; ++j)
            rows[nrow][j] = 0;

        /* Apply recurrence for m = 1..6. */
        for (idx_t m = 1; m <= 6; ++m) {
            if (m > nrow) break;                 /* P(x,n-m) would be negative */

            if (m & 1U) {                        /* odd m: (m+1)*x^m*P(x,n-m) */
                seq_t mul = (seq_t)(m + 1);
                for (idx_t d = 0; d < nrow - m; ++d) {
                    rows[nrow][d + m] += mul * rows[nrow - m][d];
                }
            } else { /* even m: (n-1)^(m/2)*P(x,n-m) */
                seq_t base   = (seq_t)(nrow - 1);
                seq_t factor = 1;
                for (idx_t t = 0; t < m / 2; ++t)
                    factor *= base;
                for (idx_t d = 0; d < nrow - m; ++d) {
                    rows[nrow][d] += factor * rows[nrow - m][d];
                }
            }
        }
    }

    /* Result is the coefficient at row r, position idx_within_row. */
    seq_t result = rows[r][idx_within_row];

    /* Free allocated memory. */
    for (idx_t k = 0; k <= r; ++k) {
        if (rows[k]) {
            seq_t_destruct(rows[k], k);
            free(rows[k]);
        }
    }
    free(rows);

    return result;
}
