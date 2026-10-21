/*
 * A063420  Coefficient array for certain numerator polynomials N3(n,x), n >=
 *          0 (rising powers of x) used for trinomials.
 * https://oeis.org/A063420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063420 – flattened coefficients of the numerator polynomials N3(n,x).
 *
 * Definition (OEIS):
 *   N3(0,x) = 1
 *   N3(1,x) = 1
 *   For n ≥ 2
 *     let c(n) = 1 if n even else 0
 *     N3(n,x) = Σ_{j=1..2} (1–x)^{j-1} * x^{b(c(n),j)} * N3(n-j,x)
 *   where b(c,j)=1 if 1≤j≤c else 0.
 *
 *   For even n: b(1,1)=1, b(1,2)=0  →  N3(n,x)=x·N3(n-1,x)+(1–x)·N3(n-2,x)
 *   For odd  n: b(0,1)=0, b(0,2)=0  →  N3(n,x)=   N3(n-1,x)+(1–x)·N3(n-2,x)
 *
 * The sequence a(n) is obtained by writing down, for each n≥0,
 * the coefficients of N3(n,x) in order of increasing powers of x
 * (omitting any trailing zeros) and concatenating those lists.
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=-1, …
 *
 * The function below computes a(n) by generating the polynomials
 * incrementally, storing all coefficients in a flat array, and
 * memoising the result.  It runs in O(n) time and O(n) space.
 */

#include <stdlib.h>

/* public function */
seq_t a(idx_t n);

/* ----- internal data ----------------------------------------------------- */
static seq_t *fl_flat = NULL;      /* flattened coefficients */
static size_t fl_len = 0;          /* number of terms stored        */
static size_t fl_cap = 0;          /* allocated capacity            */

static seq_t *poly_prev = NULL;    /* N3(n-1,x) coefficients (owned)  */
static seq_t *poly_prev2 = NULL;   /* N3(n-2,x) coefficients (owned)  */
static size_t len_prev = 0;        /* length of poly_prev           */
static size_t len_prev2 = 0;       /* length of poly_prev2           */
static idx_t last_n = 0;           /* largest n for which coeffs are valid */

/* ----- helper for flattened array --------------------------------------- */
static void fl_ensure(size_t needed)
{
    if (needed > fl_cap) {
        size_t cap = (fl_cap ? fl_cap * 2 : 16);
        while (cap < needed) cap <<= 1;
        fl_flat = (seq_t *)realloc(fl_flat, cap * sizeof(seq_t));
        fl_cap = cap;
    }
}

static void fl_append(const seq_t *coeff, size_t len)
{
    fl_ensure(fl_len + len);
    for (size_t i = 0; i < len; ++i)
        fl_flat[fl_len + i] = coeff[i];
    fl_len += len;
}

/* ----- polynomial construction ------------------------------------------ */
static seq_t *poly_even(const seq_t *P, size_t lenP,
                        const seq_t *Q, size_t lenQ, size_t *out_len)
{
    size_t new_len = (lenP > lenQ ? lenP : lenQ) + 1;
    seq_t *res = (seq_t *)malloc(new_len * sizeof(seq_t));
    for (size_t i = 0; i < new_len; ++i) res[i] = 0;

    /* x·P  → shift left by 1 */
    for (size_t i = 0; i < lenP; ++i)
        res[i + 1] += P[i];

    /* (1–x)·Q = Q – x·Q */
    for (size_t i = 0; i < lenQ; ++i) {
        res[i]     += Q[i];
        res[i + 1] -= Q[i];
    }

    /* trim trailing zeros */
    size_t trimmed = new_len;
    while (trimmed > 0 && res[trimmed - 1] == 0) --trimmed;
    if (trimmed == 0) trimmed = 1;                /* safety; should not happen */

    if (trimmed != new_len) {
        seq_t *tmp = (seq_t *)realloc(res, trimmed * sizeof(seq_t));
        if (tmp) res = tmp;
        new_len = trimmed;
    }

    *out_len = new_len;
    return res;
}

static seq_t *poly_odd(const seq_t *P, size_t lenP,
                       const seq_t *Q, size_t lenQ, size_t *out_len)
{
    size_t new_len = (lenP > lenQ + 1 ? lenP : lenQ + 1);
    seq_t *res = (seq_t *)malloc(new_len * sizeof(seq_t));
    for (size_t i = 0; i < new_len; ++i) res[i] = 0;

    /* P (no shift) */
    for (size_t i = 0; i < lenP; ++i)
        res[i] += P[i];

    /* (1–x)·Q = Q – x·Q */
    for (size_t i = 0; i < lenQ; ++i) {
        res[i]     += Q[i];
        res[i + 1] -= Q[i];
    }

    /* trim trailing zeros */
    size_t trimmed = new_len;
    while (trimmed > 0 && res[trimmed - 1] == 0) --trimmed;
    if (trimmed == 0) trimmed = 1;                /* safety */

    if (trimmed != new_len) {
        seq_t *tmp = (seq_t *)realloc(res, trimmed * sizeof(seq_t));
        if (tmp) res = tmp;
        new_len = trimmed;
    }

    *out_len = new_len;
    return res;
}

/* ----- build next polynomial -------------------------------------------- */
static void build_next(idx_t n)
{
    size_t new_len;
    seq_t *poly;

    if ((n & 1) == 0)
        poly = poly_even(poly_prev, len_prev, poly_prev2, len_prev2, &new_len);
    else
        poly = poly_odd (poly_prev, len_prev, poly_prev2, len_prev2, &new_len);

    fl_append(poly, new_len);

    free(poly_prev2);
    poly_prev2 = poly_prev;
    len_prev2 = len_prev;
    poly_prev = poly;
    len_prev = new_len;
    last_n = n;
}

/* ----- public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* first‑time initialization */
    if (fl_flat == NULL) {
        /* N3(0) = [1] */
        seq_t *p0 = (seq_t *)malloc(sizeof(seq_t));
        p0[0] = (seq_t)1;
        fl_append(p0, 1);
        poly_prev2 = p0;
        len_prev2 = 1;

        /* N3(1) = [1] */
        seq_t *p1 = (seq_t *)malloc(sizeof(seq_t));
        p1[0] = (seq_t)1;
        fl_append(p1, 1);
        poly_prev = p1;
        len_prev = 1;
        last_n = 1;
    }

    /* generate terms until we have at least n+1 flattened entries */
    while (fl_len <= (size_t)n)
        build_next(last_n + 1);

    return fl_flat[(size_t)n];
}
