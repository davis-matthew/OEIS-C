/*
 * A133563  Expansion of chi(-q) / chi(-q^5) in powers of q where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A133563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in ∏_{k>0} (1 + q^{5k})/(1 + q^k)
 * Range:     i8 n <= 69, i16 n <= 192, i32 n <= 588, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by multiplying the generating function term‑by‑term and
   performing the required convolution.  The function is memoised and
   recomputed only when larger indices are requested.  */
#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cached values a[0..a_cap-1] */
static size_t   a_cap   = 0;     /* size of the cache (highest index + 1) */

/* Compute the sequence up to index N (inclusive) and return a newly
   allocated array containing a[0]..a[N].  The caller owns the
   memory and must free it after the next recomputation.  */
static seq_t *compute_up_to(idx_t N)
{
    size_t size = (size_t)N + 1;

    /* temporary arrays */
    seq_t *A = (seq_t*)calloc(size, sizeof(seq_t));   /* A[n]: partitions into distinct multiples of 5 */
    seq_t *B = (seq_t*)calloc(size, sizeof(seq_t));   /* B[n]: coefficients of 1 / ∏(1+q^k)          */
    seq_t *P = (seq_t*)calloc(size, sizeof(seq_t));   /* P[n]: partitions into distinct parts         */
    seq_t *a = (seq_t*)calloc(size, sizeof(seq_t));   /* final sequence                           */

    /* --- Partitions into distinct parts:  P[n] --- */
    P[0] = 1;
    for (size_t part = 1; part <= N; ++part) {
        for (size_t d = N; d >= part; --d) {
            P[d] += P[d - part];
        }
    }

    /* --- Coefficients B[n] of the reciprocal:  B[0] = 1,
           B[n] = - Σ_{k=1}^{n} P[k] * B[n-k] --- */
    B[0] = 1;
    for (size_t nidx = 1; nidx <= N; ++nidx) {
        seq_t sum = 0;
        for (size_t k = 1; k <= nidx; ++k) {
            sum += P[k] * B[nidx - k];
        }
        B[nidx] = -sum;
    }

    /* --- Partitions into distinct multiples of 5:  A[n] --- */
    A[0] = 1;
    for (size_t mult = 5; mult <= N; mult += 5) {
        for (size_t d = N; d >= mult; --d) {
            A[d] += A[d - mult];
        }
    }

    /* --- Convolution: a[n] = Σ_{k=0}^{n} A[n-k] * B[k] --- */
    for (size_t nidx = 0; nidx <= N; ++nidx) {
        seq_t s = 0;
        for (size_t k = 0; k <= nidx; ++k) {
            s += A[nidx - k] * B[k];
        }
        a[nidx] = s;
    }

    /* free temporary arrays */
    free(A);
    free(B);
    free(P);

    return a;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    if ((size_t)n < a_cap) {
        return a_cache[n];
    }

    /* Need to recompute up to n */
    seq_t *new_cache = compute_up_to(n);

    free(a_cache);
    a_cache = new_cache;
    a_cap   = (size_t)n + 1;

    return a_cache[n];
}
