/*
 * A037947  Coefficients of unique normalized cusp form Delta_26 of weight 26
 *          for full modular group.
 * https://oeis.org/A037947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in ((E4(q)^3 - E6(q)^2)/12^3) * E6(q)
 *            * E4(q)^2 where E4(q)=1+240∑_{n≥1} σ₃(n) q^n and
 *            E6(q)=1-504∑_{n≥1} σ₅(n) q^n
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 34, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */

/* Ensure the code compiles in both C11 and C++17 */
#if defined(__cplusplus)
  #include <new>           /* for operator new[] / delete[]  */
#endif

/* Temporary storage for intermediate series. */
static idx_t max_n = 0;
static seq_t *E4        = NULL;
static seq_t *E6        = NULL;
static seq_t *E4sq      = NULL;
static seq_t *E4cu      = NULL;
static seq_t *E6sq      = NULL;
static seq_t *diff      = NULL;
static seq_t *E6E4sq    = NULL;
static seq_t *final_arr = NULL;
static seq_t *sigma3    = NULL;
static seq_t *sigma5    = NULL;

/* Free an array allocated by new[] (C++) or malloc (C). */
static void free_array(seq_t *p) {
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* Allocate an array of (N+1) seq_t elements. */
static seq_t *alloc_array(idx_t N) {
#if defined(__cplusplus)
    return new seq_t[N + 1];
#else
    return (seq_t*)malloc((N + 1) * sizeof(seq_t));
#endif
}

/* Zero‑initialise an array of seq_t elements. */
static void zero_array(seq_t *p, idx_t N) {
    for (idx_t i = 0; i <= N; ++i) {
        p[i] = (seq_t)0;
    }
}

/* Convolution of two series a and b up to order N, result in c. */
static void mul_series(const seq_t *a, const seq_t *b, seq_t *c, idx_t N) {
    for (idx_t i = 0; i <= N; ++i) c[i] = (seq_t)0;
    for (idx_t i = 0; i <= N; ++i) {
        seq_t ai = a[i];
        if (ai == (seq_t)0) continue;
        for (idx_t j = 0; j <= N - i; ++j) {
            c[i + j] += ai * b[j];
        }
    }
}

/* Recompute all auxiliary series up to order N. */
static void recompute(idx_t N) {
    /* free old allocations */
    if (E4        != NULL) free_array(E4);
    if (E6        != NULL) free_array(E6);
    if (E4sq      != NULL) free_array(E4sq);
    if (E4cu      != NULL) free_array(E4cu);
    if (E6sq      != NULL) free_array(E6sq);
    if (diff      != NULL) free_array(diff);
    if (E6E4sq    != NULL) free_array(E6E4sq);
    if (final_arr != NULL) free_array(final_arr);
    if (sigma3    != NULL) free_array(sigma3);
    if (sigma5    != NULL) free_array(sigma5);

    /* allocate new arrays */
    E4        = alloc_array(N);
    E6        = alloc_array(N);
    E4sq      = alloc_array(N);
    E4cu      = alloc_array(N);
    E6sq      = alloc_array(N);
    diff      = alloc_array(N);
    E6E4sq    = alloc_array(N);
    final_arr = alloc_array(N);
    sigma3    = alloc_array(N);
    sigma5    = alloc_array(N);

    /* zero all arrays */
    zero_array(E4,    N);
    zero_array(E6,    N);
    zero_array(E4sq,  N);
    zero_array(E4cu,  N);
    zero_array(E6sq,  N);
    zero_array(diff,  N);
    zero_array(E6E4sq,N);
    zero_array(final_arr, N);
    zero_array(sigma3, N);
    zero_array(sigma5, N);

    /* divisor sums σ₃ and σ₅ */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t dd   = (seq_t)d;
        seq_t dd2  = dd * dd;
        seq_t dd3  = dd2 * dd;        /* d^3 */
        seq_t dd5  = dd3 * dd2;        /* d^5 */
        for (idx_t n = d; n <= N; n += d) {
            sigma3[n] += dd3;
            sigma5[n] += dd5;
        }
    }

    /* Eisenstein series E4 and E6 */
    E4[0] = (seq_t)1;
    E6[0] = (seq_t)1;
    for (idx_t n = 1; n <= N; ++n) {
        E4[n] = (seq_t)240 * sigma3[n];
        E6[n] = -(seq_t)504 * sigma5[n];
    }

    /* E4^2, E4^3, E6^2 */
    mul_series(E4,   E4,   E4sq, N);
    mul_series(E4sq, E4,   E4cu, N);
    mul_series(E6,   E6,   E6sq, N);

    /* (E4^3 - E6^2)/12^3  (12^3 = 1728) */
    const seq_t DIV = 1728;
    for (idx_t i = 0; i <= N; ++i) {
        diff[i] = (E4cu[i] - E6sq[i]) / DIV;
    }

    /* E6 * E4^2 */
    mul_series(E6,   E4sq, E6E4sq, N);

    /* Final product: diff * E6E4sq */
    mul_series(diff, E6E4sq, final_arr, N);

    max_n = N;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence offset is 1 */
    if (n > max_n) recompute(n);
    return final_arr[n];
}
