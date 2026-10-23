/*
 * A016182  Inverse of 2173rd cyclotomic polynomial.
 * https://oeis.org/A016182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the expansion of 1/Φ_2173(x). The
 *            sequence is periodic with period 2173. It is computed once by a
 *            linear recurrence derived from the cyclotomic polynomial Φ_2173
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *a_seq  = NULL;   /* periodic values, size 2173 */
static seq_t *phi    = NULL;   /* coefficients of Φ_2173, size 2081 */
static const size_t PERIOD = 2173;
static const size_t DEG_PHI = 2080;      /* degree of Φ_2173 */
static bool initialized = false;

/* Helper to compute the coefficients of Φ_2173 = (x^2173-1)/((x^53-1)(x^41-1))*(x-1) */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Part 1: build (x^2173-1) */
    const size_t DEG_P = 2173;
    seq_t *P = (seq_t *)calloc(DEG_P + 1, sizeof(seq_t));
    if (!P) abort();
    P[0]  = -(seq_t)1;
    P[DEG_P] = (seq_t)1;

    /* Part 2: build denominator (x^53-1)*(x^41-1) = x^94 - x^53 - x^41 + 1 */
    const size_t DEG_D = 94;
    seq_t *D = (seq_t *)calloc(DEG_D + 1, sizeof(seq_t));
    if (!D) abort();
    D[0]   = (seq_t)1;
    D[41]  = -(seq_t)1;
    D[53]  = -(seq_t)1;
    D[94]  = (seq_t)1;

    /* Part 3: perform long division P = Q * D */
    const size_t DEG_Q = DEG_P - DEG_D;   /* 2079 */
    seq_t *Q = (seq_t *)calloc(DEG_Q + 1, sizeof(seq_t));
    if (!Q) abort();

    for (size_t i = DEG_P; i >= DEG_D; --i) {
        seq_t coef = P[i];
        if (coef == 0) continue;
        size_t shift = i - DEG_D;
        Q[shift] = coef;
        /* subtract coef * D shifted by 'shift' from P */
        for (size_t j = 0; j <= DEG_D; ++j) {
            /* idx = shift + j <= i */
            P[shift + j] -= coef * D[j];
        }
    }
    /* After division P should be all zeros; we ignore the remainder. */
    free(P);
    free(D);

    /* Part 4: multiply Q by (x-1) to obtain Φ_2173 = (x-1) * Q */
    phi = (seq_t *)calloc(DEG_PHI + 1, sizeof(seq_t));
    if (!phi) abort();

    /* (x-1)*Q: coeff at x^k is -Q[k] + Q[k-1] (k>=1), 
       coeff at x^0 is -Q[0], coeff at x^DEG_Q+1 is +Q[DEG_Q] */
    phi[0]          = -(seq_t)Q[0];
    for (size_t k = 1; k <= DEG_Q; ++k) {
        phi[k] = -Q[k] + Q[k - 1];
    }
    phi[DEG_PHI] = Q[DEG_Q];

    free(Q);

    /* Part 5: build the periodic sequence a_seq of length PERIOD (2173) */
    a_seq = (seq_t *)calloc(PERIOD, sizeof(seq_t));
    if (!a_seq) abort();

    a_seq[0] = (seq_t)1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = (seq_t)0;
        size_t maxk = (n < DEG_PHI) ? n : DEG_PHI;
        for (size_t k = 1; k <= maxk; ++k) {
            sum += phi[k] * a_seq[n - k];
        }
        a_seq[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    return a_seq[ (size_t)(n % PERIOD) ];
}
