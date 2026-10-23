/*
 * A135763  Expansion of (theta_3(q) * theta_3(q^3))^3 in powers of q.
 * https://oeis.org/A135763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* • A135763: expansion of (theta_3(q) * theta_3(q^3))^3 in powers of q
   • a(n) = coefficient of q^n in (∑_{i∈ℤ} q^{i^2} · ∑_{j∈ℤ} q^{3j^2})³
   • Computation: let c(k) = #solutions of i²+3j² = k (i,j ∈ ℤ);
     then a(n) = (c * c * c)(n), i.e. the triple convolution of c. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build. */
static seq_t   *C = NULL;      /* c values     */
static seq_t   *D = NULL;      /* d = c*c      */
static seq_t   *A = NULL;      /* a = d*c      */
static idx_t    MaxComputed = 0;  /* number of terms already computed (length of arrays) */

/* forward declaration of helper to create arrays */
static void compute_terms(idx_t n);

/* public entry point */
seq_t a(idx_t n)
{
    if (n >= MaxComputed) compute_terms(n);
    return A[n];
}

/* ------------------------------------------------------------------------- */
/* compute all terms up to n (inclusive), store in global arrays C, D, A     */
static void compute_terms(idx_t n)
{
    /* allocate new arrays */
    idx_t size = n + 1;
    seq_t *c_new = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *d_new = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *a_new = (seq_t *)calloc((size_t)size, sizeof(seq_t));

    /* --- compute c(k)  ----------------------------------------------- */
    for (idx_t i = 0; i <= n; ++i) {
        idx_t ii = i * i;
        if (ii > n) break;
        for (idx_t j = 0; ; ++j) {
            /* value of i^2 + 3j^2 */
            idx_t val = ii + 3ULL * j * j;
            if (val > n) break;
            int mult;
            if (i == 0 && j == 0)           mult = 1;
            else if (i == 0 || j == 0)      mult = 2;
            else                            mult = 4;
            c_new[val] += (seq_t)mult;
        }
    }

    /* --- compute d = c * c  ------------------------------------------- */
    for (idx_t i = 0; i <= n; ++i) {
        for (idx_t j = 0; j <= n - i; ++j) {
            d_new[i + j] += (seq_t)c_new[i] * (seq_t)c_new[j];
        }
    }

    /* --- compute a = d * c  ------------------------------------------- */
    for (idx_t i = 0; i <= n; ++i) {
        for (idx_t j = 0; j <= n - i; ++j) {
            a_new[i + j] += (seq_t)d_new[i] * (seq_t)c_new[j];
        }
    }

    /* free old arrays */
    if (C) free(C);
    if (D) free(D);
    if (A) free(A);

    /* store new arrays and size */
    C   = c_new;
    D   = d_new;
    A   = a_new;
    MaxComputed = size;
}
