/*
 * A014324  Inverse of 315th cyclotomic polynomial.
 * https://oeis.org/A014324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the nth coefficient of the formal power series expansion
 *            of 1/Φ_315(x), where Φ_315 is the 315th cyclotomic polynomial.
 *            The coefficients are periodic with period 315. The
 *            implementation computes the polynomial coefficients once and
 *            then evaluates the sequence by the recurrence a(0) = 1, a...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Polynomial manipulation:  multiply/divide by (x^d-1)                */
/* ------------------------------------------------------------------ */

#define MAX_DEG 400            /* must be > 360 (max intermediate degree) */
static seq_t poly_temp[MAX_DEG + 1];

static void multiply_by_xd_minus1(seq_t *poly, size_t *deg, idx_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg + d;
    /* initialise temporary array to zero */
    for (size_t i = 0; i <= new_deg; ++i)
        poly_temp[i] = 0;
    /* carry out the multiplication */
    for (size_t i = 0; i <= old_deg; ++i) {
        poly_temp[i]     -= poly[i];      /* multiply by -1    */
        poly_temp[i + d] += poly[i];      /* shift by d and add*/
    }
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = poly_temp[i];
    *deg = new_deg;
}

static void divide_by_xd_minus1(seq_t *poly, size_t *deg, idx_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg - d;                 /* exact division      */
    seq_t q[MAX_DEG + 1];                         /* quotient buffer    */
    for (size_t i = 0; i <= new_deg; ++i) {
        if (i < (size_t)d)
            q[i] = -poly[i];                     /* base case          */
        else
            q[i] = q[i - d] - poly[i];           /* recurrence         */
    }
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = q[i];
    *deg = new_deg;
}

/* ------------------------------------------------------------------ */
/*  Build the 315th cyclotomic polynomial Φ_315(x)                      */
/* ------------------------------------------------------------------ */

#define PERIOD 315

static seq_t phi_coeff[MAX_DEG + 1];
static size_t phi_deg = 0;          /* degree of Φ_315(x) */

static void build_phi_315(void)
{
    /* Coefficient array for the polynomial in construction */
    seq_t poly[MAX_DEG + 1];
    size_t deg = 0;                /* currently deg(poly) = 0 */
    poly[0] = 1;                   /* start with 1 */

    /* Numerator factors (μ = +1) */
    const idx_t numerators[] = {15, 21, 9, 315};
    const size_t num_numerators = sizeof(numerators) / sizeof(numerators[0]);

    /* Denominator factors (μ = -1) */
    const idx_t denominators[] = {3, 105, 45, 63};
    const size_t num_denominators = sizeof(denominators) / sizeof(denominators[0]);

    /* Build the product of numerators */
    for (size_t i = 0; i < num_numerators; ++i)
        multiply_by_xd_minus1(poly, &deg, numerators[i]);

    /* Divide by each denominator factor (exact division) */
    for (size_t i = 0; i < num_denominators; ++i)
        divide_by_xd_minus1(poly, &deg, denominators[i]);

    /* Store the resulting coefficients */
    phi_deg = deg;
    for (size_t i = 0; i <= deg; ++i)
        phi_coeff[i] = poly[i];
}

/* ------------------------------------------------------------------ */
/*  Compute the sequence values a(0)…a(PERIOD-1) and cache them         */
/* ------------------------------------------------------------------ */

static seq_t seq_cache[PERIOD];
static bool seq_initialized = false;

static void build_sequence(void)
{
    /* Ensure Φ_315 has been constructed */
    if (phi_deg == 0)
        build_phi_315();

    /* a(0) = 1 */
    seq_cache[0] = 1;

    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t limit = n < phi_deg ? (size_t)n : phi_deg;
        for (size_t i = 1; i <= limit; ++i)
            sum += phi_coeff[i] * seq_cache[n - i];
        seq_cache[n] = -sum;            /* recurrence a(n) = -∑ Φ_i a(n-i) */
    }
    seq_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the evaluation framework               */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!seq_initialized)
        build_sequence();
    return seq_cache[n % PERIOD];
}
