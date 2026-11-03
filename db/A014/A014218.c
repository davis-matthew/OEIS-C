/*
 * A014218  Inverse of 209th cyclotomic polynomial.
 * https://oeis.org/A014218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 961 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
   For n ≥ 1,
        a(n) = – Σ_{k=1}^{180} c_k · a(n–k),
   where Φ_209(x) = Σ_{k=0}^{180} c_k x^k is the 209th cyclotomic
   polynomial (c_0 = 1).  The sequence is periodic with period 209. */

#include <stddef.h>   /* size_t */

static void compute_coeffs(int coeff[181])
{
    /* Degree of (x^11−1)(x^19−1) */
    const int DEG_D = 30;
    /* Degree of x^210−x^209−x+1 */
    const int DEG_N = 210;
    /* Degree of Φ_209(x) */
    const int DEG_Q = 180;

    /* Build D(x) = (x^11−1)(x^19−1) */
    int D[DEG_D + 1];
    for (int i = 0; i <= DEG_D; ++i) D[i] = 0;
    D[0]    = 1;
    D[11]   = -1;
    D[19]   = -1;
    D[30]   = 1;

    /* Build N(x) = (x − 1)(x^209 − 1) = x^210 − x^209 − x + 1 */
    int N[DEG_N + 1];
    for (int i = 0; i <= DEG_N; ++i) N[i] = 0;
    N[0]    = 1;
    N[1]    = -1;
    N[209]  = -1;
    N[210]  = 1;

    /* Long division N / D → Q = Φ_209 */
    int A[DEG_N + 1];
    for (int i = 0; i <= DEG_N; ++i) A[i] = N[i];

    int Q[DEG_Q + 1];
    for (int i = 0; i <= DEG_Q; ++i) Q[i] = 0;

    for (int i = DEG_N; i >= DEG_D; --i) {
        int q = A[i];
        if (q != 0) {
            Q[i - DEG_D] = q;          /* coefficient for x^(i-DEG_D) */
            for (int t = 0; t <= DEG_D; ++t) {
                A[i - DEG_D + t] -= q * D[t];
            }
        }
    }

    /* Store the coefficients (c_0 … c_180). */
    for (int i = 0; i <= DEG_Q; ++i) coeff[i] = Q[i];
}

/* ------------------------------------------------------------------ */
/* Compute the coefficients c_k of Φ_209(x) (k = 0 … 180) once.  The
   coefficients are small integers (−1, 0, 1), so we use `int` for the
   temporary array.  They are stored in `coeff[k]`.                     */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Periodic table of sequence values (period length 209).  The table
   is filled on the first call to a().                                 */
/* ------------------------------------------------------------------ */

static seq_t periodic_values[209];
static int  coeffs[181];
static int  init_done = 0;    /* 0 = not yet initialised, 1 = done */

/* ------------------------------------------------------------------ */
/* Main function required by the evaluator.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) {
        /* Compute polynomial coefficients once. */
        compute_coeffs(coeffs);

        /* Compute the first 209 terms using the recurrence. */
        periodic_values[0] = 1;                /* a(0) = 1 */

        for (idx_t i = 1; i < (idx_t)209; ++i) {
            seq_t sum = 0;
            unsigned int ii = (unsigned int)i;
            for (int k = 1; k <= 180 && (unsigned int)k <= ii; ++k) {
                sum += (seq_t)coeffs[k] * periodic_values[i - (idx_t)k];
            }
            periodic_values[i] = -sum;
        }
        init_done = 1;
    }

    /* Periodic extension: a(n) = a(n mod 209). */
    return periodic_values[(idx_t)(n % 209)];
}
