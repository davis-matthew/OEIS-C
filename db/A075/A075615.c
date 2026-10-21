/*
 * A075615  Let P(k,X) = Product_{i=1..2*k} (X-1/cos(Pi*(2*i-1)/(4*k)) ) which
 *          is a polynomial with integer coefficients. Sequence gives array of
 *          coefficients for P(k,X).
 * https://oeis.org/A075615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = coefficient placed in the b-file of the sequence * *
 *            A075615, the coefficients of P(k, X) * * This implementation
 *            evaluates the coefficient by interpreting * * the sequence as
 *            the coefficients of (-1)^k * T_{2k}(X), where * * T_n is the
 *            Chebyshev polynomial of the first kind. * * For a giv...
 * Range:     i8 n <= 19, i16 n <= 58, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the sequence type.  The build system supplies
   seq_t and idx_t. */
typedef idx_t idx_t;

typedef struct {
    size_t deg;    /* Degree of the polynomial (highest power). */
    seq_t *coeff;  /* coeff[i] is the coefficient of X^i. */
} poly_t;

/* Cache for Chebyshev polynomials T_n, indexed by n (0 ≤ n ≤ current). */
static poly_t *cheb = NULL;
static size_t cheb_cap = 0;   /* current number of stored polynomials */

/* Ensure that Chebyshev polynomials up to degree max_deg are available. */
static void ensure_cheb(idx_t max_deg)
{
    if (max_deg < (idx_t)cheb_cap)
        return;

    size_t new_cap = (size_t)max_deg + 1;
    poly_t *tmp = (poly_t *)realloc(cheb, new_cap * sizeof(poly_t));
    if (!tmp)
        abort();   /* Out of memory */
    cheb = tmp;

    /* Initialise base polynomials T_0 and T_1 if the cache was empty. */
    if (cheb_cap == 0) {
        /* T_0 = 1 */
        cheb[0].deg   = 0;
        cheb[0].coeff = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cheb[0].coeff)
            abort();
        cheb[0].coeff[0] = (seq_t)1;

        /* T_1 = X */
        cheb[1].deg   = 1;
        cheb[1].coeff = (seq_t *)calloc(2, sizeof(seq_t));
        if (!cheb[1].coeff)
            abort();
        cheb[1].coeff[0] = (seq_t)0;
        cheb[1].coeff[1] = (seq_t)1;

        cheb_cap = 2;
    }

    /* Build remaining polynomials up to max_deg using the recurrence   */
    /* T_{n+1} = 2 X T_n - T_{n-1}.                                    */
    for (size_t n = cheb_cap; n <= (size_t)max_deg; ++n) {
        if (n < 2)
            continue;   /* T_0 and T_1 already initialised */

        poly_t *T_n   = &cheb[n - 1];
        poly_t *T_nm1 = &cheb[n - 2];

        size_t degree = n;                     /* degree of T_n+1   */
        size_t len    = degree + 1;            /* number of coeffs  */
        seq_t *coeff  = (seq_t *)calloc(len, sizeof(seq_t));
        if (!coeff)
            abort();

        /* 2 X * T_n */
        for (size_t i = 0; i <= T_n->deg; ++i)
            coeff[i + 1] += (seq_t)2 * T_n->coeff[i];

        /* - T_{n-1} */
        for (size_t i = 0; i <= T_nm1->deg; ++i)
            coeff[i] -= T_nm1->coeff[i];

        cheb[n].deg   = degree;
        cheb[n].coeff = coeff;
    }

    cheb_cap = new_cap;
}

/* Return the n-th term of the sequence A075615 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n = 0, return 0 as placeholder */

    /* Find the row k such that S(k-1) < n ≤ S(k), where
       S(k) = k(k+2) is the cumulative length up to row k. */
    idx_t k = 1;
    while ((k * (k + 2)) < n)
        ++k;

    /* Position within this row (1‑based). */
    idx_t prev = (k - 1) * (k + 1);   /* S(k-1) */
    idx_t pos  = n - prev;            /* 1 … 2k+1 */

    /* Coefficients at odd positions are zero. */
    if ((pos & 1) == 0)
        return (seq_t)0;

    /* t is the index of the even power (0 ≤ t ≤ k). */
    idx_t t = (pos - 1) / 2;

    /* Ensure that T_{2k} has been computed. */
    ensure_cheb(2 * k);

    seq_t coeff = cheb[2 * k].coeff[2 * t];

    /* Apply the factor (-1)^k. */
    if (k & 1)
        coeff = -coeff;

    return coeff;
}
