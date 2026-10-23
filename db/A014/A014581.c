/*
 * A014581  Inverse of 572nd cyclotomic polynomial.
 * https://oeis.org/A014581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Sequence A014581 – Inverse of the 572nd cyclotomic polynomial.
 *  The series 1/Φ₅₇₂(x) = Σ a(n) xⁿ has period 572.  This routine
 *  computes the coefficients a(n) for all n by first constructing
 *  the polynomial Φ₅₇₂(x) (degree 240), then inverting the series
 *  up to one full period.  The returned value is a(n) for any
 *  n ≥ 0, independent of call order.
 */
#include <stddef.h>   /* size_t   */
#include <stdbool.h>  /* bool, true, false */

/* ------------------------------------------------------------ */
/*  Constants & data                                          */
/* ------------------------------------------------------------ */

#define NUM_DIVISORS     12
#define MAX_POLY         573   /* 572 + 1  (degree 572 maximum)  */
#define PERIOD           572

/* Divisors of 572 in increasing order. */
static const uint64_t divisors[NUM_DIVISORS] = {
    1ULL, 2ULL, 4ULL, 11ULL, 13ULL, 22ULL, 26ULL, 44ULL, 52ULL,
    143ULL, 286ULL, 572ULL
};

/* Polynomial coefficients for Φ_d(x).  coeff[i][0..deg] holds
 * the coefficients for divisor divisors[i].
 */
static seq_t coeff[NUM_DIVISORS][MAX_POLY];
static size_t deg[NUM_DIVISORS];   /* actual degree of Φ_d */

/* One period of the inverse series.  series[0] = a(0), series[571] = a(571). */
static seq_t series[PERIOD];
static bool initialized = false;

/* ------------------------------------------------------------ */
/* Helper: polynomial division  a(x) / b(x)   (b(x) monic).  */
/* ------------------------------------------------------------ */
static void poly_div(const seq_t *a, size_t len_a,
                     const seq_t *b, size_t len_b,
                     seq_t *q, size_t *len_q)
{
    size_t deg_a = len_a - 1;
    size_t deg_b = len_b - 1;
    size_t deg_q = deg_a - deg_b;
    size_t qlen = deg_q + 1;

    seq_t rem[MAX_POLY];
    for (size_t i = 0; i < len_a; ++i)
        rem[i] = a[i];

    for (int i = (int)deg_a; i >= (int)deg_b; --i) {
        seq_t coef = rem[i];
        size_t qi = i - deg_b;          /* position in quotient */
        q[qi] = coef;
        /* subtract coef * b(x) * x^qi from remainder   */
        for (size_t j = 0; j <= deg_b; ++j)
            rem[qi + j] -= coef * b[j];
    }
    *len_q = qlen;
}

/* ------------------------------------------------------------ */
/* Helper: compute all Φ_d for d | 572.                         */
/* ------------------------------------------------------------ */
static void compute_cyclotomics(void)
{
    /* Φ_1(x) = x - 1  (degree 1) */
    deg[0] = 1;
    for (size_t i = 0; i < MAX_POLY; ++i)
        coeff[0][i] = 0ULL;
    coeff[0][0] = -1;  /* constant term */
    coeff[0][1] =  1;  /* x term   */

    /* For each higher divisor d */
    for (size_t idx = 1; idx < NUM_DIVISORS; ++idx) {
        uint64_t d = divisors[idx];
        size_t len_d = d + 1;

        seq_t tmp[MAX_POLY];
        seq_t div_temp[MAX_POLY];
        size_t len_div = len_d;

        /* x^d - 1 */
        for (size_t i = 0; i < MAX_POLY; ++i)
            tmp[i] = 0;
        tmp[0] = -1;
        tmp[d] =  1;

        /* Divide by all proper divisors Φ_e, e | d, e < d */
        for (size_t j = 0; j < idx; ++j) {
            if (d % divisors[j] == 0) {
                poly_div(tmp, len_div,
                         coeff[j], deg[j] + 1,
                         div_temp, &len_div);
                for (size_t i = 0; i < len_div; ++i)
                    tmp[i] = div_temp[i];
            }
        }

        /* Store the resulting polynomial Φ_d.  It's monic. */
        deg[idx] = len_div - 1;
        for (size_t i = 0; i < MAX_POLY; ++i)
            coeff[idx][i] = 0;
        for (size_t i = 0; i < len_div; ++i)
            coeff[idx][i] = tmp[i];
    }
}

/* ------------------------------------------------------------ */
/* Helper: compute one period of the inverse series.           */
/* ------------------------------------------------------------ */
static void compute_inverse_series(void)
{
    const size_t phi_idx = NUM_DIVISORS - 1;   /* index of divisor 572 */
    size_t d = deg[phi_idx];                  /* degree of Φ_572 */

    /* b[0] = 1  */
    series[0] = 1;
    for (size_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        size_t upper = (k <= d) ? k : d;
        for (size_t i = 1; i <= upper; ++i)
            sum += coeff[phi_idx][i] * series[k - i];
        series[k] = -sum;
    }
}

/* ------------------------------------------------------------ */
/* One–time initialization of all tables.                       */
/* ------------------------------------------------------------ */
static void init_seq(void)
{
    if (initialized)
        return;

    compute_cyclotomics();
    compute_inverse_series();

    initialized = true;
}

/* ------------------------------------------------------------ */
/* Public function:  seq_t a(idx_t n)                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized)
        init_seq();

    idx_t pos = n % PERIOD;
    return series[(size_t)pos];
}
