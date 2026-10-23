/*
 * A015110  Triangle of q-binomial coefficients for q=-3.
 * https://oeis.org/A015110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   Triangle of q‑binomial coefficients for q = -3. Let r =
 *            ⌊(√(8n+1)−1)/2⌋ and k = n − r(r+1)/2. Then a(n) = C_{r,k} where
 *            C_{0,0} = 1, C_{r,0} = C_{r,r} = 1, C_{r,k} = C_{r-1,k} +
 *            (‑3)^(r‑k) · C_{r-1,k‑1}
 * Range:     i8 n <= 16, i16 n <= 29, i32 n <= 48, i64 n <= 95, i128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>   /* for sqrt */

static seq_t *pows = NULL;          /* pows[e] = (‑3)^e  */
static idx_t maxPow = 0;

static seq_t **coefs = NULL;        /* coefs[r][k] = q‑binomial coefficient */
static idx_t maxR = 0;

/* Ensure that powers of ‑3 up to exponent e are available. */
static void ensure_pows(idx_t e)
{
    if (e <= maxPow) return;
    seq_t *newp = (seq_t *)realloc(pows, (size_t)(e + 1) * sizeof(seq_t));
    if (!newp) abort();
    pows = newp;
    if (maxPow == 0) {
        pows[0] = (seq_t)1;
    }
    for (idx_t i = maxPow + 1; i <= e; ++i) {
        pows[i] = pows[i - 1] * (seq_t)-3;
    }
    maxPow = e;
}

/* Ensure that rows up to r are computed and stored.  Row 0 is defined
   as a single element 1. */
static void ensure_coefs(idx_t r)
{
    if (r <= maxR) return;

    seq_t **newc = (seq_t **)realloc(coefs, (size_t)(r + 1) * sizeof(seq_t *));
    if (!newc) abort();
    coefs = newc;

    ensure_pows(r);

    for (idx_t i = maxR + 1; i <= r; ++i) {
        seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row) abort();
        row[0] = row[i] = (seq_t)1;
        for (idx_t k = 1; k < i; ++k) {
            row[k] = coefs[i - 1][k] + pows[i - k] * coefs[i - 1][k - 1];
        }
        coefs[i] = row;
    }

    if (maxR == 0 && coefs[0] == NULL) {
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        if (!row0) abort();
        row0[0] = (seq_t)1;
        coefs[0] = row0;
    }

    maxR = r;
}

seq_t a(idx_t n)
{
    /* Compute the row r and column k for index n. */
    double tr = sqrt((double)(8ULL * n + 1ULL));
    idx_t r = (idx_t)((tr - 1.0) / 2.0);
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Lazy initialization of base data. */
    if (coefs == NULL) {
        coefs = (seq_t **)malloc(sizeof(seq_t *));
        if (!coefs) abort();
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        if (!row0) abort();
        row0[0] = (seq_t)1;
        coefs[0] = row0;
        maxR = 0;
    }
    if (pows == NULL) {
        pows = (seq_t *)malloc(sizeof(seq_t));
        if (!pows) abort();
        pows[0] = (seq_t)1;
        maxPow = 0;
    }

    ensure_coefs(r);
    return coefs[r][k];
}
