/*
 * A171657  Coefficients of a set of infinite sum rational polynomials:
 *          p(x,n)=(-1 + x)^(m - 1)*( 1 - (1 + x)/(-1 + x))^(m + 1)*Sum[(k +
 *          1)^(2*m - 1)*((x + 1)/( x - 1))^k, {k, 0, Infinity}].
 * https://oeis.org/A171657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, uned
 * Formula:   a(n) = coefficient of x^{n-(m-1)^2-1} in p_m(x) where m =
 *            ceil(sqrt(n)) and P_m(x) =
 *            (-2)^{1-m}·(x-1)^{2m-2}·A_{2m-1}\!\!\left(\frac{x+1}{x-1}\right)
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 40, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t euler[200][200];
static seq_t binom[201][201];
static seq_t pow2[201];
static bool tables_inited = false;

/* Precompute Eulerian numbers, binomial coefficients and powers of 2 */
static void init_tables(void)
{
    if (tables_inited) return;
    /* powers of 2 */
    pow2[0] = 1;
    for (int i = 1; i <= 200; ++i) pow2[i] = pow2[i-1] * (seq_t)2;
    /* binomial coefficients */
    for (int n = 0; n <= 200; ++n) {
        binom[n][0] = binom[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }
    /* Eulerian numbers: A(0,0)=1; A(n,k)= (k+1)A(n-1,k)+(n-k)A(n-1,k-1) */
    euler[0][0] = 1;
    for (int n = 1; n <= 199; ++n) {
        for (int k = 0; k < n; ++k) {
            seq_t term1 = (seq_t)(k+1) * euler[n-1][k];
            seq_t term2 = (seq_t)(n-k) * euler[n-1][k-1];
            euler[n][k] = term1 + term2;
        }
    }
    tables_inited = true;
}

/* Integer square root: floor(sqrt(x)) */
static idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    for (int b = 63; b >= 0; --b) {
        idx_t t = r | ((idx_t)1 << b);
        if ((unsigned __int128)t * t <= (unsigned __int128)x) {
            r = t;
        }
    }
    return r;
}

/* Return a(n) for the OEIS sequence A171657 */
seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return 0;          /* undefined but harmless */
    /* find m such that (m-1)^2 < n <= m^2 */
    idx_t m = isqrt(n-1) + 1;
    idx_t pos  = n - (m-1) * (m-1);   /* 1‑based position in the row */
    idx_t p    = pos - 1;             /* degree of the coefficient (0‑based) */

    /* Long intermediate sum N = sum_{j,s} ...  (see analysis) */
    seq_t N = 0;
    idx_t two_m_minus_1 = 2*m - 1;
    idx_t two_m_minus_2 = 2*m - 2;

    for (idx_t j = 0; j <= two_m_minus_1; ++j) {
        if (j == two_m_minus_1) continue;      /* A(2m-1, 2m-1) == 0 */
        seq_t A = euler[two_m_minus_1][j];
        if (A == 0) continue;
        for (idx_t s = 0; s <= j; ++s) {
            if (p > two_m_minus_2 - s) continue;   /* binomial zero */
            seq_t C_js  = binom[j][s];
            seq_t twop  = pow2[s];
            seq_t C_qp  = binom[two_m_minus_2 - s][p];
            int id     = (int)(m-1) - (int)s - (int)p;
            seq_t sign = (id & 1) ? -1 : 1;
            seq_t term = A * C_js;
            term *= twop;
            term *= C_qp;
            term *= sign;
            N += term;
        }
    }
    seq_t denom = pow2[m-1];
    return N / denom;
}
