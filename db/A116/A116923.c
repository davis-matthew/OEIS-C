/*
 * A116923  Triangle T(n,k) = Sum_{i=0..k}
 *          (-1)^(i+k)*binomial(k,i)*Sum_{j=0..n} (i+1)^j*(3n-3j+1) read by
 *          rows.
 * https://oeis.org/A116923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where row r and column k satisfy * r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, k = n - r(r+1)/2 * and T(r,k) = Σ_{i=0..k}
 *            (-1)^(i+k) * C(k,i) * * Σ_{j=0..r} (i+1)^j * (3r-3j+1). * The
 *            function computes the triangle values on demand and returns the
 *            * entry read by rows, matching OEIS A116923
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ---------- internal data structures ---------- */
static seq_t **triangle = NULL;      /* triangle[row][k] */
static size_t  triangle_rows = 0;    /* number of rows that have been computed */

/* ---------- binomial coefficient (multiplicative) ---------- */
static seq_t binom_idx(idx_t k, idx_t i)
{
    seq_t res = 1;
    for (idx_t d = 1; d <= i; ++d) {
        res = res * (seq_t)(k - d + 1) / (seq_t)d;
    }
    return res;
}

/* ---------- compute a single row of the triangle ---------- */
static void compute_row(idx_t r)
{
    seq_t *row = NULL;
    seq_t *S   = NULL;

#if defined(__cplusplus)
    row = new seq_t[r + 1];
    S   = new seq_t[r + 1];
#else
    row = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
    S   = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
#endif
    if (!row || !S) {
        /* Error handling – exit for this demonstration. */
        exit(EXIT_FAILURE);
    }

    /* S[i] = Σ_{j=0..r} (i+1)^j * (3r-3j+1) */
    for (idx_t i = 0; i <= r; ++i) {
        seq_t pow = 1;
        seq_t sum = 0;
        for (idx_t j = 0; j <= r; ++j) {
            seq_t fac = (seq_t)(3 * r - 3 * j + 1);
            sum += pow * fac;
            pow *= (seq_t)(i + 1);
        }
        S[i] = sum;
    }

    /* T(r,k) = Σ_{i=0..k} (-1)^(i+k) * C(k,i) * S[i] */
    for (idx_t k = 0; k <= r; ++k) {
        seq_t pos = 0, neg = 0;
        for (idx_t i = 0; i <= k; ++i) {
            seq_t coeff = binom_idx(k, i);
            seq_t term  = coeff * S[i];
            if (((i + k) & 1) == 0)
                pos += term;
            else
                neg += term;
        }
        row[k] = pos - neg;
    }

#if !defined(__cplusplus)
    free(S);
#else
    delete[] S;
#endif

    triangle[r] = row;
    triangle_rows = r + 1;   /* rows 0..r are now available */
}

/* ---------- ensure rows up to and including r are computed ---------- */
static void ensure_rows(idx_t r)
{
    if (!triangle) {
        triangle = (seq_t**)malloc((size_t)(r + 1) * sizeof(seq_t*));
        if (!triangle) exit(EXIT_FAILURE);
    } else if (triangle_rows <= r) {
        triangle = (seq_t**)realloc(triangle, (size_t)(r + 1) * sizeof(seq_t*));
        if (!triangle) exit(EXIT_FAILURE);
    }
    for (idx_t cur = triangle_rows; cur <= r; ++cur) {
        compute_row(cur);
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Locate the row that contains the nth term. */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n) {
        ++row;
    }

    /* Column within that row. */
    idx_t start_of_row = row * (row + 1) / 2;
    idx_t k = n - start_of_row;

    /* Compute any missing rows lazily. */
    ensure_rows(row);

    return triangle[row][k];
}
