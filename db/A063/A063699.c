/*
 * A063699  Positions of negative coefficients in cyclotomic polynomial
 *          Phi_n(x), A063698 in binary.
 * https://oeis.org/A063699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 34, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A063699: Positions of negative coefficients in the
 *  cyclotomic polynomial Φₙ(x).  For each n ≥ 0, a(n) is the
 *  decimal integer obtained by writing the binary pattern of
 *  negative coefficient positions as digits.
 *  Example: Φ₁₅(x) has negatives at positions 1,4,7 → pattern 10010010 → a(15)=10010010.
 *
 *  This implementation constructs cyclotomic polynomials by the
 *  recursive divisor formula, memoises them, and converts the
 *  binary pattern to a decimal integer without relying on bit
 *  shifting beyond the limits of seq_t.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Polynomial representation: monic, coefficient array ∈ int64_t, index = exponent */
typedef struct { size_t deg; int64_t *c; } poly_t;

/* Memoisation tables */
static poly_t *polys     = NULL;   /* array of poly_t, indices 0..n */
static char   *computed  = NULL;   /* 0/1 flags per index   */
static size_t  polys_size = 0;     /* current allocated size */

/* Ensure tables can hold the index n */
static void ensure_size(idx_t n)
{
    if ((size_t)n >= polys_size) {
        size_t newsize = ((size_t)n + 1);
        polys    = (poly_t *)realloc(polys,    newsize * sizeof(poly_t));
        computed = (char  *)realloc(computed, newsize * sizeof(char));
        for (size_t i = polys_size; i < newsize; ++i) {
            polys[i].deg = 0;
            polys[i].c   = NULL;
            computed[i]  = 0;
        }
        polys_size = newsize;
    }
}

/* Polynomial long division: A / B  (both monic).  Returns quotient in a newly
   allocated poly_t structure; caller owns the returned poly_t and its array.
   The polynomial representation used is a[n] * x^n + ... + a[0]. */
static poly_t *poly_divide(const poly_t *A, const poly_t *B)
{
    size_t m = A->deg, d = B->deg;
    if (m < d) {
        /* Zero quotient */
        poly_t *res = (poly_t *)malloc(sizeof(poly_t));
        res->deg = 0;
        res->c   = NULL;
        return res;
    }

    size_t qdeg = m - d;
    int64_t *R = (int64_t *)malloc((m + 1) * sizeof(int64_t));
    int64_t *Q = (int64_t *)calloc(qdeg + 1, sizeof(int64_t));
    memcpy(R, A->c, (m + 1) * sizeof(int64_t));

    for (int i = (int)m; i >= (int)d; --i) {
        int64_t coeff = R[i];
        if (coeff != 0) {
            Q[i - d] += coeff;          /* quotient term */
            for (size_t j = 0; j <= d; ++j)
                R[i - d + j] -= coeff * B->c[j];
            R[i] = 0;
        }
    }

    size_t newdeg = 0;
    for (size_t i = 0; i <= qdeg; ++i)
        if (Q[i]) newdeg = i;

    poly_t *res = (poly_t *)malloc(sizeof(poly_t));
    res->deg = newdeg;
    res->c   = Q;
    free(R);
    return res;
}

/* Recursive construction of the cyclotomic polynomial Φₙ(x) */
static poly_t *getPhi(idx_t n)
{
    if (n == 0) return NULL;   /* Φ₀ not used */

    ensure_size(n);
    if (computed[n]) return &polys[n];

    if (n == 1) {                /* Φ₁(x) = x - 1 */
        polys[1].deg = 1;
        polys[1].c   = (int64_t *)malloc(2 * sizeof(int64_t));
        polys[1].c[0] = -1;
        polys[1].c[1] =  1;
        computed[1] = 1;
        return &polys[1];
    }

    /* Start with xⁿ - 1 */
    poly_t A;
    A.deg = n;
    A.c   = (int64_t *)calloc(n + 1, sizeof(int64_t));
    A.c[0] = -1;
    A.c[n] =  1;

    /* Divide by Φ_d for every proper divisor d of n */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d != 0) continue;
        poly_t *Pd = getPhi(d);
        poly_t *Q  = poly_divide(&A, Pd);
        free(A.c);               /* free previous numerator */
        A.deg = Q->deg;
        A.c   = Q->c;            /* take ownership of coeff array */
        free(Q);                 /* free struct only */
    }

    polys[n].deg = A.deg;
    polys[n].c   = A.c;
    computed[n]  = 1;
    return &polys[n];
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    poly_t *P = getPhi(n);

    /* Find the highest exponent with a negative coefficient */
    bool found = false;
    size_t highest = 0;
    for (size_t i = 0; i <= P->deg; ++i) {
        if (P->c[i] < 0) {
            if (!found || i > highest) {
                highest = i;
                found   = true;
            }
        }
    }

    if (!found) return 0;  /* all coefficients non‑negative */

    seq_t res = 0;
    for (size_t i = highest; ; --i) {
        seq_t bit = (P->c[i] < 0) ? 1 : 0;
        res = res * 10 + bit;
        if (i == 0) break;   /* avoid underflow on unsigned */
    }

    return res;
}
