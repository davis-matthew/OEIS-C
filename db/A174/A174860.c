/*
 * A174860  A triangle sequence of polynomial
 *          coefficients:p(x,n)=Sum[Eulerian[n+1, k]*(-x)^k*Sum[StirlingS2[n,
 *          m]*x^m, {m, 0, n - k}], {k, 0, n}].
 * https://oeis.org/A174860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   a(n) = coefficient t(r,m) of the triangle where r =
 *            floor((sqrt(8n+1)-1)/2) (row index) m = n - r*(r+1)/2 (position
 *            in the row) t(r,m) = Σ_{i=0}^{min(r,m)} (-1)^i * Eulerian(r+1,i)
 *            * StirlingS2(r,m-i)
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef MAX_R
#define MAX_R 200                     /* enough for n up to ~20000 */
#endif

static seq_t euler[MAX_R + 2][MAX_R + 2];   /* Eulerian numbers A(n,k) */
static seq_t stirling[MAX_R + 1][MAX_R + 1];/* Stirling numbers of the 2nd kind S(n,k) */
static bool tables_initialized = false;

/* Build tables of Eulerian and Stirling numbers up to MAX_R */
static void init_tables(void)
{
    if (tables_initialized) return;
    tables_initialized = true;

    /* Eulerian numbers: E(1,0)=1, for n>1 E(n,0)=1 */
    for (idx_t n = 0; n <= MAX_R + 1; ++n)
        for (idx_t k = 0; k <= MAX_R + 1; ++k)
            euler[n][k] = 0;

    euler[1][0] = 1;
    for (idx_t n = 2; n <= MAX_R + 1; ++n) {
        euler[n][0] = 1;
        for (idx_t k = 1; k < n; ++k) {
            seq_t term1 = (seq_t)(k + 1) * euler[n - 1][k];
            seq_t term2 = (seq_t)(n - k) * euler[n - 1][k - 1];
            euler[n][k] = term1 + term2;
        }
    }

    /* Stirling numbers of the 2nd kind: S(0,0)=1 */
    for (idx_t n = 0; n <= MAX_R; ++n)
        for (idx_t k = 0; k <= MAX_R; ++k)
            stirling[n][k] = 0;

    stirling[0][0] = 1;
    for (idx_t n = 1; n <= MAX_R; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = (seq_t)k * stirling[n - 1][k];
            seq_t term2 = stirling[n - 1][k - 1];
            stirling[n][k] = term1 + term2;
        }
    }
}

/* Compute coefficient t(r,m) for the triangle */
static seq_t t_coeff(idx_t r, idx_t m)
{
    seq_t sum = 0;
    idx_t lim = r < m ? r : m;
    for (idx_t i = 0; i <= lim; ++i) {
        seq_t prod = euler[r + 1][i] * stirling[r][m - i];
        if (i & 1U) prod = -prod;
        sum += prod;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 1;

    /* find r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t m = n - offset;

    return t_coeff(r, m);
}
