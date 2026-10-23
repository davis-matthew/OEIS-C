/*
 * A181997  G.f. satisfies: x = Sum_{n>=1} 1/A(x)^(3*n) * Product_{k=1..n} (1
 *          - 1/A(x)^k).
 * https://oeis.org/A181997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS A181997):
 *      A(x) = 1 + x + 2*x^2 + 9*x^3 + 46*x^4 + …
 *      A(x) is the compositional inverse of the polynomial
 *      f(z) = z − 2*z^2 − z^3 + 4*z^4 + 4*z^5 + z^6,
 *      i.e. f( A(x) − 1 ) = x .
 *  The following computes the n‑th coefficient of A(x).
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* For use by the build system:
 *  typedef  <definition of idx_t>  idx_t;
 *  typedef  <definition of seq_t>  seq_t;
 *  No need (and not allowed) to redeclare these types here. */

/* Static storage for the power series coefficients. */
static seq_t *g     = NULL;  /* g[n] = coefficient of x^n in the reduced series A(x)-1   */
static seq_t *pow2  = NULL;  /* coeffs of (g)^2                             */
static seq_t *pow3  = NULL;  /* coeffs of (g)^3                             */
static seq_t *pow4  = NULL;  /* coeffs of (g)^4                             */
static seq_t *pow5  = NULL;  /* coeffs of (g)^5                             */
static seq_t *pow6  = NULL;  /* coeffs of (g)^6                             */
static size_t  capacity  = 0;   /* allocated length (maximum n that can be stored)   */
static size_t  computed  = 0;   /* highest n for which coefficients are computed   */

/* Allocate or re‑allocate the coefficient arrays up to new_cap elements. */
static void
ensure_capacity(size_t new_cap)
{
    if (new_cap <= capacity)
        return;

    /* Allocate new arrays, zero‑initialised. */
    seq_t *ng  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    seq_t *n2  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    seq_t *n3  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    seq_t *n4  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    seq_t *n5  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    seq_t *n6  = (seq_t*)calloc(new_cap, sizeof(seq_t));

    /* Copy old data into the new arrays and free the old ones. */
    if (g != NULL) {
        memcpy(ng, g,  capacity * sizeof(seq_t));
        free(g);
    }
    if (pow2 != NULL) {
        memcpy(n2, pow2, capacity * sizeof(seq_t));
        free(pow2);
    }
    if (pow3 != NULL) {
        memcpy(n3, pow3, capacity * sizeof(seq_t));
        free(pow3);
    }
    if (pow4 != NULL) {
        memcpy(n4, pow4, capacity * sizeof(seq_t));
        free(pow4);
    }
    if (pow5 != NULL) {
        memcpy(n5, pow5, capacity * sizeof(seq_t));
        free(pow5);
    }
    if (pow6 != NULL) {
        memcpy(n6, pow6, capacity * sizeof(seq_t));
        free(pow6);
    }

    g    = ng;
    pow2 = n2;
    pow3 = n3;
    pow4 = n4;
    pow5 = n5;
    pow6 = n6;

    capacity = new_cap;
}

/* Compute all coefficients up to n (inclusive). */
static void
compute_upto(idx_t n)
{
    if ((size_t)n <= computed)
        return;

    size_t N = (size_t)n;
    ensure_capacity(N + 1);          /* indices 0 … N */

    /* Initialise the first coefficient (g[1] = 1). */
    if (computed < 1) {
        g[1] = (seq_t)1;            /* g[0] remains 0 */
        /* All pow arrays are already zero-initialised. */
        computed = 1;
    }

    /* Coefficients of f(z) = z + f2*z^2 + ... + f6*z^6  */
    const seq_t f2 = (seq_t)(-2);
    const seq_t f3 = (seq_t)(-1);
    const seq_t f4 = (seq_t)4;
    const seq_t f5 = (seq_t)4;
    const seq_t f6 = (seq_t)1;

    /* For each n from 2 up to N compute the new coefficients. */
    for (size_t i = 2; i <= N; ++i) {
        seq_t sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;

        /* Compute coefficients of g^2 … g^6 at degree i. */
        for (size_t k = 1; k < i; ++k) {
            seq_t gk = g[k];
            seq_t gik = g[i - k];

            sum2 += gk * gik;            /* g * g */
            sum3 += gk * pow2[i - k];    /* g * g^2 */
            sum4 += gk * pow3[i - k];    /* g * g^3 */
            sum5 += gk * pow4[i - k];    /* g * g^4 */
            sum6 += gk * pow5[i - k];    /* g * g^5 */
        }

        pow2[i] = sum2;
        pow3[i] = sum3;
        pow4[i] = sum4;
        pow5[i] = sum5;
        pow6[i] = sum6;

        /* Solve g[i] from f(g) = 0 for degrees ≥ 2:   */
        /* g[i] + f2*pow2[i] + f3*pow3[i] + f4*pow4[i] + f5*pow5[i] + f6*pow6[i] = 0 */
        seq_t temp = f2 * pow2[i] + f3 * pow3[i] + f4 * pow4[i] + f5 * pow5[i] + f6 * pow6[i];
        g[i] = -temp;                    /* negate modulo 2^bits */
    }

    computed = N;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if ((size_t)n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    compute_upto((size_t)n);
    return g[(size_t)n];                 /* a(n) = g[n] for n ≥ 1 */
}
