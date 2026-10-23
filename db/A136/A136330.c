/*
 * A136330  A triangular sequence from 2^n times the coefficients of
 *          characteristic polynomials of a rational tridiagonal matrix type:
 *          M(3)= {{1/2,-1,0} {-1,1/2,-m}, {0,-1,1/2}}};m=-1; polynomial
 *          recursion associated is: p(x, n) = (1 - 2*x)*p(x, n - 1)/2 - p(x,
 *          n - 2);.
 * https://oeis.org/A136330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 59, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136330
   Triangular array of coefficients of 2^k · P_k(x),
   where the monic polynomials P_k satisfy
        P_0(x) = 1,
        P_1(x) = x – 1/2,
        P_k(x) = (x – 1/2)·P_{k‑1}(x) – P_{k‑2}(x)   (k ≥ 2).
   Each row r (starting with r = 1) is the coefficients of
   (-1)^{r‑1}·P_{r‑1}(x) written from constant term up to the
   highest power.  Thus the nth element of the linearised sequence
   is the coefficient of the polynomial corresponding to the row
   containing n. */

/* The function below computes any term a(n) for n ≥ 1.  No external
   libraries are required.  The implementation keeps all arithmetic
   in the type seq_t, which may be any signed integer type or an
   arbitrary–precision integer class. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r that contains index n. */
    idx_t r   = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev_cum = cum - r;     /* total terms before this row   */
    idx_t pos      = n - prev_cum;/* 1‑based position inside row   */
    idx_t deg      = r - 1;       /* degree of the polynomial      */

    /* Base case: first row. */
    if (deg == 0) return 1;

    /* Maximum degree that will be needed for n ≤ 10000 is < 141;
       a conservative upper bound of 200 is safe for all compilations. */
    const idx_t MAXSIZE = 200;
    seq_t prev2[MAXSIZE];
    seq_t prev1[MAXSIZE];
    seq_t cur [MAXSIZE];

    /* Initialise all arrays to zero.  The loops that follow
       only touch the part of the arrays that is currently
       in use. */
    for (idx_t i = 0; i < MAXSIZE; ++i) {
        prev2[i] = 0;
        prev1[i] = 0;
        cur[i]   = 0;
    }

    /* Q_0 = 1  ;  Q_1 = 2x – 1  (coefficients in constant‑first order). */
    prev2[0] = 1;          /* Q_0 */
    prev1[0] = -1;         /* Q_1 constant term  */
    prev1[1] =  2;         /* Q_1 coefficient of x */

    /* If only two first rows are required, we can finish early. */
    if (deg == 1) {
        seq_t val = prev1[pos - 1];
        if (deg & 1) val = -val;   /* odd degree rows are negated */
        return val;
    }

    /* Build Q_k up to the required degree using the recurrence
       Q_k = (2x – 1)·Q_{k‑1} – 4·Q_{k‑2}.  All computations are
       carried out in seq_t. */
    for (idx_t k = 2; k <= deg; ++k) {
        /* cur holds Q_k and has length k+1. */
        for (idx_t i = 0; i <= k; ++i) cur[i] = 0;

        /* (2x – 1)·prev1 */
        for (idx_t i = 0; i < k; ++i) {
            cur[i]   -= prev1[i];          /* constant part of multiplication by -1 */
            cur[i+1] += (seq_t)2 * prev1[i];/* coefficient of x obtained from 2x * prev1 */
        }

        /* subtract 4·prev2 */
        for (idx_t i = 0; i < k - 1; ++i) {
            cur[i] -= (seq_t)4 * prev2[i];
        }

        /* rotate the buffers for the next iteration. */
        for (idx_t i = 0; i <= k; ++i) {
            prev2[i] = prev1[i];  /* Q_{k-1} becomes previous previous */
            prev1[i] = cur[i];    /* Q_k becomes previous   */
        }
    }

    /* Retrieve the desired coefficient from Q_deg.
       For odd degrees the row is multiplied by -1. */
    seq_t result = prev1[pos - 1];
    if (deg & 1) result = -result;
    return result;
}
