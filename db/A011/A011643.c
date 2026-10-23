/*
 * A011643  84th cyclotomic polynomial.
 * https://oeis.org/A011643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 84th cyclotomic polynomial
 *            Φ_84(x)
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Constants --------------------------------------------------------------- */
static const idx_t N                 = (idx_t)84ULL;      /* n in Φ_n(x) */
static const idx_t SUM_POS_T         = (idx_t)108ULL;     /* 84+14+6+4 */
static const size_t DP_SIZE          = (size_t)SUM_POS_T + 1U; /* 109 » max intermediate degree */

/* Helper: multiply polynomial by (x^t – 1) -------------------------------- */
static void mul_xt_minus1(seq_t *poly, idx_t *deg, idx_t t)
{
    idx_t old_deg = *deg;
    seq_t newres[DP_SIZE];

    /* zero initialise new result up to the new degree */
    for (idx_t i = 0; i <= old_deg + t; ++i) {
        newres[i] = 0;
    }

    /* newres = poly * (x^t – 1) */
    for (idx_t i = 0; i <= old_deg; ++i) {
        newres[i]     -= poly[i];      /* multiply by –1 */
        newres[i + t] += poly[i];      /* multiply by x^t */
    }

    *deg = old_deg + t;
    for (idx_t i = 0; i <= *deg; ++i) {
        poly[i] = newres[i];
    }
}

/* Helper: divide polynomial by (x^t – 1) --------------------------------- */
static void div_xt_minus1(seq_t *poly, idx_t *deg, idx_t t)
{
    idx_t old_deg = *deg;
    idx_t new_deg = old_deg - t;
    seq_t quo[DP_SIZE];

    /* i < t : Q[i] = –P[i] */
    for (idx_t i = 0; i < t; ++i) {
        quo[i] = -poly[i];
    }

    /* i ≥ t : Q[i] = Q[i–t] – P[i] */
    for (idx_t i = t; i <= new_deg; ++i) {
        quo[i] = quo[i - t] - poly[i];
    }

    /* copy quotient back to polynomial */
    for (idx_t i = 0; i <= new_deg; ++i) {
        poly[i] = quo[i];
    }
    *deg = new_deg;
}

/* Cache for the coefficients of Φ_84(x) ----------------------------------- */
static seq_t poly_coef[25];
static int   poly_ready = 0;

/* -------------------------------------------------------------------------- */
/* seq_t a(idx_t n) ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Compute coefficients once */
    if (!poly_ready) {
        seq_t poly[DP_SIZE];
        idx_t deg = 0;

        for (idx_t i = 0; i < DP_SIZE; ++i)
            poly[i] = 0;
        poly[0] = 1;        /* start with 1 */

        /* Multiply by (x^t – 1) for divisors with μ(d) = +1 */
        mul_xt_minus1(poly, &deg, (idx_t)84ULL);  /* d = 1  */
        mul_xt_minus1(poly, &deg, (idx_t)14ULL);  /* d = 6  */
        mul_xt_minus1(poly, &deg, (idx_t) 6ULL);  /* d =14  */
        mul_xt_minus1(poly, &deg, (idx_t) 4ULL);  /* d =21  */

        /* Divide by (x^t – 1) for divisors with μ(d) = –1 */
        div_xt_minus1(poly, &deg, (idx_t)42ULL);  /* d = 2  */
        div_xt_minus1(poly, &deg, (idx_t)28ULL);  /* d = 3  */
        div_xt_minus1(poly, &deg, (idx_t)12ULL);  /* d = 7  */
        div_xt_minus1(poly, &deg, (idx_t) 2ULL);  /* d =42  */

        /* Store coefficients up to degree φ(84) = 24 */
        for (idx_t i = 0; i <= 24; ++i)
            poly_coef[i] = poly[i];
        poly_ready = 1;
    }

    if (n <= 24)
        return poly_coef[(size_t)n];
    return 0;
}
