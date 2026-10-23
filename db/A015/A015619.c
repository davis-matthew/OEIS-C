/*
 * A015619  Inverse of 1610th cyclotomic polynomial.
 * https://oeis.org/A015619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A015619 – Inverse of the 1610th cyclotomic polynomial
 *
 *  The sequence is the coefficient sequence of the formal power series
 *  1 / Φ_1610(x).  For a square‑free integer n the coefficients of
 *  Φ_n(x) are ±1 or 0.  They are given by
 *
 *      Φ_n(x) = ∏_{d|n} (x^d – 1)^{μ(n/d)},
 *
 *  where μ is the Möbius function.
 *
 *  The recurrence for the inverse series follows from
 *
 *      Φ_1610(x) · ∑_{k≥0} a(k) x^k = 1
 *
 *  .  For n ≥ 1
 *
 *      a(n) = – Σ_{i=1}^{φ(1610)} Φ_i · a(n–i)      (1)
 *
 *  where Φ_i is the i‑th coefficient of Φ_1610(x) and Φ_0 = 1.
 *
 *  The sequence is periodic with period 1610.  The following implementation
 *  computes the Φ_1610(x) coefficients once and then generates the a(n)
 *  values on demand, caching the period.  The algorithm uses only
 *  integer arithmetic on seq_t and respects all of the constraints.
 */

#include <stdlib.h>

#define PERIOD          1610UL                /* period of the sequence          */
#define PHI_LEN         529UL                /* φ(1610) + 1 = 528 + 1           */
#define MAX_POLY_LEN    2000UL               /* largest intermediate length     */

static seq_t *ACACHE = NULL;               /* cache of one period             */

/* --- Polynomial multiplication by (x^d − 1) ---------------------------- */
static void mul_by_factor(seq_t *poly, size_t *len_ptr, idx_t d) {
    size_t len  = *len_ptr;
    size_t newlen = len + (size_t)d;
    seq_t temp[MAX_POLY_LEN];

    for (size_t i = 0; i < newlen; ++i) {
        seq_t val = 0;
        if (i >= (size_t)d && (i - (size_t)d) < len)
            val += poly[i - (size_t)d];
        if (i < len)
            val -= poly[i];
        temp[i] = val;
    }
    for (size_t i = 0; i < newlen; ++i)
        poly[i] = temp[i];
    *len_ptr = newlen;
}

/* --- Polynomial division by (x^d − 1) --------------------------------- */
static void div_by_factor(seq_t *poly, size_t *len_ptr, idx_t d) {
    size_t len  = *len_ptr;
    size_t newlen = len - (size_t)d;
    seq_t temp[MAX_POLY_LEN];

    /* for indices < d:  –h[i] = poly[i]   ⇒   h[i] = –poly[i]  */
    for (size_t i = 0; i < (size_t)d; ++i)
        temp[i] = -poly[i];

    /* recurrence  h[i] = h[i−d] − poly[i]   for i ≥ d            */
    for (size_t i = (size_t)d; i < len; ++i)
        temp[i] = temp[i - (size_t)d] - poly[i];

    for (size_t i = 0; i < newlen; ++i)
        poly[i] = temp[i];
    *len_ptr = newlen;
}

/* --- Compute the coefficients of Φ_1610(x) ----------------------------- */
static void compute_phi_coeffs(seq_t *phi) {
    /* 1610 = 2·5·7·23  (square‑free, 4 prime factors)              */
    /*  Divisors where μ(n/d) = +1 (even number of remaining primes) */
    static const idx_t POS_D[8] = {1,10,14,35,46,115,161,1610};
    /*  Divisors where μ(n/d) = –1 (odd number of remaining primes) */
    static const idx_t NEG_D[8] = {2,5,7,23,70,230,322,805};

    seq_t poly[MAX_POLY_LEN] = {0};
    size_t len = 1;          /* start with the constant polynomial 1   */
    poly[0] = 1;

    for (size_t k = 0; k < 8; ++k)
        mul_by_factor(poly, &len, POS_D[k]);

    for (size_t k = 0; k < 8; ++k)
        div_by_factor(poly, &len, NEG_D[k]);

    /* len must now be φ(1610)+1 = 529                         */
    for (size_t i = 0; i < PHI_LEN; ++i)
        phi[i] = poly[i];
}

/* --- Build the sequence cache (period of 1610) ------------------------ */
static void ensure_cache(void) {
    if (ACACHE)
        return;

    ACACHE = (seq_t *)malloc(PERIOD * sizeof(seq_t));
    if (!ACACHE)         /* allocation failed – caller will get 0          */
        return;

    /* 1.  Φ_1610(x) coefficients  */
    seq_t phi[PHI_LEN];
    compute_phi_coeffs(phi);          /* phi[0] = 1, phi[1..] are ±1 or 0            */

    /* 2.  a(0) … a(1609) using the recurrence (1) */
    ACACHE[0] = 1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t limit = (n < (size_t)PHI_LEN - 1) ? (size_t)n : (size_t)PHI_LEN - 1;
        for (size_t i = 1; i <= limit; ++i)
            sum += phi[i] * ACACHE[n - (idx_t)i];
        ACACHE[n] = -sum;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    ensure_cache();
    return ACACHE[n % PERIOD];
}
