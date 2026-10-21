/*
 * A202185  Triangle T(n,m) = coefficient of x^n in expansion of
 *          x^m*(x+1)^(log(1+x)*m) = sum(n>=m, T(n,m) x^n*m!/n!).
 * https://oeis.org/A202185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 28, i32 n <= 66, i64 n <= 172,
 *            i128 n <= 497, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202185
 *  a(n) is the flattened triangle T(n,m) where
 *  T(n,m) = (-1)^(n-m) * binomial(n,m) *
 *           sum_{k=0}^{n-m} ((2k)!/k! * m^k * S(n-m,2k))
 *  with S(t,2k) the unsigned Stirling numbers of the first kind.
 *  The mapping from index n to (row=m, column=m) is:
 *  let r be the smallest integer with r(r+1)/2 >= n,
 *  let m = n - r(r-1)/2,
 *  then a(n) = T(r,m).
 */

#include <stdbool.h>

/* Types seq_t and idx_t are provided by the build environment. */

/* maximum required row for n up to about 10000 */
#define MAX_ROW 200
#define FACT_SIZE (2*MAX_ROW+1)

/* pre‑computed tables */
static seq_t fact[FACT_SIZE];
static seq_t stir[MAX_ROW+1][MAX_ROW+1];
static seq_t binom[MAX_ROW+1][MAX_ROW+1];
static bool initialized = false;

/* initialise factorials, Stirling numbers of the first kind (unsigned) and binomials */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i < FACT_SIZE; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }

    /* unsigned Stirling numbers of the first kind */
    stir[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        stir[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            /* stir[n][k] = stir[n-1][k-1] + (n-1)*stir[n-1][k] */
            seq_t term1 = stir[n-1][k-1];
            seq_t term2 = stir[n-1][k] * (seq_t)(n-1);
            stir[n][k] = term1 + term2;
        }
    }

    /* binomial coefficients using Pascal's triangle */
    binom[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        binom[n][0] = binom[n][n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }
}

/* flattening of triangle T(n,m) to a single sequence */
seq_t a(idx_t n)
{
    init_tables();

    /* find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev_tri = (r * (r - 1)) / 2;
    idx_t m = n - prev_tri;          /* 1 <= m <= r */
    idx_t t = r - m;                 /* t = n - m */

    seq_t sumS = (seq_t)0;
    seq_t powm = (seq_t)1;           /* m^k */

    for (idx_t k = 0; k <= t; ++k) {
        idx_t two_k = 2 * k;
        if (two_k > t) break;        /* S(t, 2k) = 0 when 2k > t */

        seq_t stir_val = stir[t][two_k];
        if (stir_val != (seq_t)0) {
            seq_t term = fact[two_k];
            term /= fact[k];         /* (2k)! / k! */
            term *= powm;            /* multiply by m^k */
            term *= stir_val;        /* multiply by S(t, 2k) */
            sumS += term;
        }
        if (k < t) powm *= m;        /* update for next k */
    }

    seq_t result = binom[r][m] * sumS;
    if (t & 1) result = -result;     /* multiply by (-1)^(n-m) */
    return result;
}
