/*
 * A136265  Integral form of A053120 :Triangle of coefficients of Integral
 *          form Chebyshev's T(n, x) polynomials (powers of x in increasing
 *          order); Much improved version by use of the integro-differential
 *          recursive form over a previous attempt.
 * https://oeis.org/A136265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136265
   Flattened triangle of coefficients of the integral form of Chebyshev
   T(n,x) polynomials.  For n≥0
         P_0(x)=1,  P_1(x)=x,  P_n=2xP_{n-1}−P_{n-2}
   And
         IP_n(x)=2xP_n(x)−(d/dx)P_{n+1}(x).
   The sequence a(n) is the flattening of the rows of IP_n in increasing
   order (row 1 contains the single coefficient of P_0).
   The function is memoised and works for all n≥1. */

#include <stdlib.h>

static seq_t **P_rows = NULL;      /* P_rows[n] holds coeffs of P_n (size n+1)  */
static idx_t  P_rows_n = 0;        /* highest n that has been computed         */

static seq_t **IP_rows = NULL;     /* IP_rows[r] holds r‑th row (size r)       */
static idx_t  IP_rows_n = 0;       /* highest r that has been computed        */

/* smallest r such that r*(r+1)/2 ≥ n  (triangular numbering) */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

/* ensure that P_rows[0…target] are computed */
static void ensure_P_rows(idx_t target)
{
    if (target < 1)          /* nothing beyond P_0 needed */
        return;

    if (P_rows == NULL) {
        /* allocate array for P_0…P_target */
        P_rows = (seq_t**)malloc((target + 1) * sizeof(seq_t*));
        if (!P_rows) abort();

        /* P_0 = 1 */
        P_rows[0] = (seq_t*)malloc(1 * sizeof(seq_t));
        if (!P_rows[0]) abort();
        P_rows[0][0] = (seq_t)1;
        P_rows_n = 0;
    }

    if (target <= P_rows_n)     /* already have enough rows */
        return;

    /* grow the pointer array if necessary */
    if ((size_t)target + 1 > 0) {
        seq_t **tmp = (seq_t**)realloc(P_rows, (target + 1) * sizeof(seq_t*));
        if (!tmp) abort();
        P_rows = tmp;
    }

    for (idx_t n = P_rows_n + 1; n <= target; ++n) {
        seq_t *coeff = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!coeff) abort();

        if (n == 1) {
            /* P_1 = x */
            coeff[0] = (seq_t)0;
            coeff[1] = (seq_t)1;
        } else { /* n >= 2 */
            seq_t *Pn_1 = P_rows[n - 1];
            seq_t *Pn_2 = P_rows[n - 2];

            coeff[0] = -Pn_2[0];          /* constant term */

            for (idx_t k = 1; k <= n; ++k) {
                seq_t term1 = (seq_t)2 * Pn_1[k - 1];
                seq_t term2 = (k <= n - 2) ? Pn_2[k] : (seq_t)0;
                coeff[k] = term1 - term2;
            }
        }
        P_rows[n] = coeff;
    }
    P_rows_n = target;
}

/* ensure that IP_rows[1…target] are computed */
static void ensure_IP_rows(idx_t target)
{
    if (target < 1)
        return;

    if (IP_rows == NULL) {
        /* allocate array for rows up to target */
        IP_rows = (seq_t**)malloc((target + 1) * sizeof(seq_t*));
        if (!IP_rows) abort();
        IP_rows_n = 0;
    }

    if (target <= IP_rows_n)
        return;

    /* grow the pointer array if necessary */
    if ((size_t)target + 1 > 0) {
        seq_t **tmp = (seq_t**)realloc(IP_rows, (target + 1) * sizeof(seq_t*));
        if (!tmp) abort();
        IP_rows = tmp;
    }

    for (idx_t r = IP_rows_n + 1; r <= target; ++r) {
        if (r == 1) {
            /* row 1: just {1} */
            seq_t *row1 = (seq_t*)malloc(1 * sizeof(seq_t));
            if (!row1) abort();
            row1[0] = (seq_t)1;
            IP_rows[1] = row1;
            continue;
        }

        idx_t n_ip = r - 2;          /* n for which IP_n is this row */
        idx_t len  = r;              /* number of coefficients in the row */

        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) abort();

        /* we need P_n and P_{n+1} */
        ensure_P_rows(n_ip + 1);

        seq_t *P_n   = P_rows[n_ip];
        seq_t *P_n1  = P_rows[n_ip + 1];

        /* coefficient for x^0 */
        row[0] = -P_n1[1];   /* Q_0 = 1 * P_{n+1}[1] */

        if (n_ip == 0) {
            /* row 2 (n=0) has only two terms */
            row[1] = (seq_t)2 * P_n[0];   /* 2*P_0[0] */
        } else {
            /* coefficients for x^k, 1 ≤ k ≤ n_ip */
            for (idx_t k = 1; k <= n_ip; ++k) {
                seq_t Qk = (seq_t)(k + 1) * P_n1[k + 1];
                row[k] = (seq_t)2 * P_n[k - 1] - Qk;
            }
            /* last coefficient x^{n+1} */
            row[n_ip + 1] = (seq_t)2 * P_n[n_ip];
        }
        IP_rows[r] = row;
    }
    IP_rows_n = target;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    idx_t r = row_of(n);                 /* row number (1-based) */
    idx_t pos = n - (r - 1) * r / 2;     /* 1-based position in that row */

    if (pos == 0) return (seq_t)0;       /* safety check */

    ensure_IP_rows(r);

    return IP_rows[r][pos - 1];
}
