/*
 * A125458  Number of base 23 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   trace(A^n) where A is the 23×23 adjacency matrix with A[i][j] =
 *            1 if |i-j| ≤ 8, else 0. a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We first compute a(0)…a(23) by repeated multiplication of A.
   Then we derive the linear recurrence of order 23 via Newton's
   identities, and finally we use this recurrence to evaluate
   a(n) for any n.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Number of states (digits 0…22). */
#define DIM 23
/* Order of the recurrence (equal to DIM). */
#define ORD 23

/* ------------------------------------------------------------------ */
/* Helper: matrix multiplication C = A * B  (all DIM×DIM matrices). */
static void mat_mul(const seq_t A[DIM][DIM],
                    const seq_t B[DIM][DIM],
                    seq_t   C[DIM][DIM])
{
    for (size_t i = 0; i < DIM; ++i) {
        for (size_t j = 0; j < DIM; ++j) {
            seq_t sum = 0;
            for (size_t k = 0; k < DIM; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Helper: compute trace of a DIM×DIM matrix. */
static seq_t mat_trace(const seq_t M[DIM][DIM])
{
    seq_t tr = 0;
    for (size_t i = 0; i < DIM; ++i) {
        tr += M[i][i];
    }
    return tr;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;          /* cache[0..max_n] */
    static size_t cache_size = 0;         /* entries stored */
    /* Recurrence coefficients: a(n) = Σ coeff[j] * a(n-1-j),  j=0..ORD-1 */
    static seq_t *coeff = NULL;           /* coeff[0]=c0, ..., coeff[ORD-1]=c_DIM-1 */
    static bool initialized = false;

    /* Lazy initialization. */
    if (!initialized) {
        /* Build the adjacency matrix A. */
        static seq_t A[DIM][DIM];
        for (size_t i = 0; i < DIM; ++i) {
            for (size_t j = 0; j < DIM; ++j) {
                if ( (i >= j && i-j <= 8) || (j > i && j-i <= 8) )
                    A[i][j] = 1;
                else
                    A[i][j] = 0;
            }
        }

        /* Compute a[0..ORD] (0..23). */
        seq_t *a_vec = (seq_t*)malloc((ORD + 1) * sizeof(seq_t));
        a_vec[0] = 1;                      /* empty string */
        a_vec[1] = mat_trace(A);           /* 23 */

        /* cur = A^k, start with k=1. */
        seq_t cur[DIM][DIM];
        for (size_t i = 0; i < DIM; ++i)
            for (size_t j = 0; j < DIM; ++j)
                cur[i][j] = A[i][j];

        for (size_t k = 2; k <= ORD; ++k) {
            /* cur = cur * A */
            seq_t nxt[DIM][DIM];
            mat_mul(cur, A, nxt);
            for (size_t i = 0; i < DIM; ++i)
                for (size_t j = 0; j < DIM; ++j)
                    cur[i][j] = nxt[i][j];
            a_vec[k] = mat_trace(cur);
        }

        /* Build recurrence coefficients using Newton's identities. */
        /* Elementary symmetric polynomials e[0..ORD].  e[0] = 1. */
        seq_t *e = (seq_t*)malloc((ORD + 1) * sizeof(seq_t));
        e[0] = 1;
        for (size_t k = 1; k <= ORD; ++k) {
            seq_t sum = a_vec[k];          /* start with p_k */
            for (size_t j = 1; j <= k-1; ++j) {
                seq_t term = e[j] * a_vec[k-j];
                if (j & 1)  /* odd j => subtract */
                    sum -= term;
                else        /* even j => add */
                    sum += term;
            }
            e[k] = sum / (idx_t)k;         /* division exact */
        }

        /* Store recurrence coefficients c[0..ORD-1] where
           λ^ORD + c[ORD-1] λ^{ORD-1} + … + c[0] = 0            */
        coeff = (seq_t*)malloc(ORD * sizeof(seq_t));
        for (size_t k = 1; k <= ORD; ++k) {
            seq_t val = e[k];
            if ( ((k & 1) == 0) )          /* (-1)^k */
                val = -val;
            coeff[ORD - k] = val;
        }

        /* Prepare cache up to ORD.  */
        cache_size = ORD + 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i <= ORD; ++i)
            cache[i] = a_vec[i];

        free(a_vec);
        free(e);

        initialized = true;
    }

    /* Extend cache if necessary. */
    idx_t required = n;
    if (required >= cache_size) {
        size_t new_size = (size_t)(required + 1);
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i <= required; ++i) {
            /* Compute a[i] using recurrence. */
            seq_t pos = 0;
            seq_t neg = 0;
            for (size_t j = 0; j < ORD; ++j) {
                seq_t term = cache[i - 1 - j];
                seq_t cval  = coeff[j];
                if (cval >= 0)
                    pos += term * cval;
                else
                    neg += term * (-cval);
            }
            cache[i] = pos - neg;      /* pos ≥ neg for non‑negative a[i] */
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
