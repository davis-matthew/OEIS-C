/*
 * A137481  Triangular sequence based on the coefficients of the magnetic
 *          model for q=1/2: p(x,t)=Exp[x*t]*((t^2 + 1/2 - 1)/(2*t + 1/2 -
 *          2))^2.
 * https://oeis.org/A137481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the n‑th term of the triangle * generated from P(x,n) in
 *            the magnetic model. * * For row r = 1,2,3,… the polynomial
 *            indices are * n = r-1 (0‑based polynomial order) * and the
 *            column index (0‑based) is * m = 0,…,n. * * Define t = n - m (t
 *            >= 0). * * Let q_t be the coefficient of t^t in * Q...
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* Return the n‑th term of A137481 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against 0 */
        return 0;

    /* Find the row number r such that r(r+1)/2 >= n. */
    double approx = sqrt((double)8 * n + 1.0);
    idx_t r = (idx_t)((approx - 1.0) / 2.0);
    while ((r * (r + 1) / 2) < n) r++;

    /* Column index k (0‑based) within the row. */
    idx_t prev = (r - 1) * r / 2;      /* sum of lengths of previous rows */
    idx_t k = n - prev - 1;            /* 0‑based column index */

    idx_t nrow = r - 1;                /* polynomial order */
    idx_t m = k;                       /* column (0‑based) */
    idx_t t = nrow - m;                /* t = n - m */

    /* Compute factorial ratio (n! / m!).  Since m <= nrow, iterate upward. */
    seq_t fac_ratio = 1;
    for (idx_t i = m + 1; i <= nrow; ++i)
        fac_ratio *= (seq_t)i;

    seq_t result;

    if (t == 0) {
        /* out = 3^nrow * fac_ratio */
        seq_t pow3 = pow_seq(3, nrow);
        result = fac_ratio * pow3;
    } else if (t == 1) {
        /* out = 8 * 3^(nrow-1) * fac_ratio */
        seq_t pow3 = pow_seq(3, nrow - 1);
        result = fac_ratio * pow3 * 8;
    } else if (t == 2) {
        /* out = 4 * 3^(nrow-1) * fac_ratio */
        seq_t pow3 = pow_seq(3, nrow - 1);
        result = fac_ratio * pow3 * 4;
    } else { /* t >= 3 */
        seq_t pow4 = pow_seq(4, t - 3);
        seq_t pow3m = pow_seq(3, m);
        result = fac_ratio * pow4 * pow3m * ((seq_t)t - 35);
    }

    return result;
}
