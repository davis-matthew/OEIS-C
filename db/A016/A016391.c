/*
 * A016391  364th cyclotomic polynomial.
 * https://oeis.org/A016391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 144, i16 n <= 144, i32 n <= 144, i64 n <= 144,
 *            i128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A016391 – coefficients of the 364th cyclotomic polynomial Φ_364(x).
    a(n) is the coefficient of x^n in Φ_364(x).  The function builds all
    cyclotomic polynomials Φ_d(x) for d | 364 recursively using the
    standard relation
        Φ_d(x) = (x^d − 1) ÷ ∏_{c|d, c<d} Φ_c(x).
    The coefficients are stored in static arrays, so a() runs in
    O(1) after the first call, and works for any signed integer type
    seq_t (including arbitrary‑precision) and for any unsigned index
    type idx_t. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for polynomials Φ_n(x) for 1 ≤ n ≤ 364.
   Each row has room for 365 coefficients (degree ≤ 364). */
static seq_t phi_coeff[365][365 + 1];
static size_t phi_deg[365] = {0};

/* Recursive computation of Φ_n(x).  The result is stored in
   phi_coeff[n] and its degree in phi_deg[n]. */
static void compute_phi(idx_t n)
{
    if (phi_deg[n] != 0 || n == 0)   /* already computed or irrelevant */
        return;

    if (n == 1) {                  /* base case Φ₁(x) = x − 1 */
        phi_coeff[1][0] = (seq_t)-1;
        phi_coeff[1][1] = (seq_t)1;
        phi_deg[1] = 1;
        return;
    }

    /* Temporary arrays for the current dividend and quotient.
       Size 365 is sufficient as the maximum degree encountered is 364. */
    seq_t temp[365 + 1] = {0};
    seq_t quotient[365 + 1] = {0};

    size_t deg = (size_t)n;
    for (size_t i = 0; i <= deg; ++i)
        temp[i] = (seq_t)0;
    temp[0]   = (seq_t)-1;    /* coefficient of x^0 (−1) */
    temp[deg] = (seq_t)1;     /* coefficient of x^deg (1) */

    /* Divide by each proper divisor Φ_d(x). */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d != 0)
            continue;

        compute_phi(d);                 /* ensure Φ_d is ready */
        size_t ddeg = phi_deg[d];

        size_t qdeg = deg - ddeg;
        for (size_t i = 0; i <= qdeg; ++i)
            quotient[i] = (seq_t)0;

        /* Long division; coefficients are processed from high to low. */
        for (int i = (int)deg; i >= (int)ddeg; --i) {
            seq_t c = temp[i];
            if (c != (seq_t)0) {
                quotient[i - (int)ddeg] = c;
                /* Subtract c * Φ_d from temp. */
                for (size_t j = 0; j <= ddeg; ++j) {
                    temp[i - ddeg + j] -= c * phi_coeff[d][j];
                }
            }
        }
        /* The remainder should vanish; copy quotient back to temp. */
        deg = qdeg;
        for (size_t i = 0; i <= deg; ++i)
            temp[i] = quotient[i];
    }

    /* Store the final polynomial. */
    for (size_t i = 0; i <= deg; ++i)
        phi_coeff[n][i] = temp[i];
    phi_deg[n] = deg;
}

/* Public sequence entry.  For indices beyond the degree of Φ_364,
   the coefficient is defined to be 0. */
seq_t a(idx_t n)
{
    /* Ensure Φ_364 is computed once. */
    if (phi_deg[364] == 0)
        compute_phi(364);

    if (n <= (idx_t)phi_deg[364])
        return phi_coeff[364][(size_t)n];
    else
        return (seq_t)0;
}
