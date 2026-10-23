/*
 * A139146  Interpolation one-half polynomials based on Chebyshev T(x.n)
 *          polynomial coefficients(A053120 ): even-> 2*T(x,n); odd->T(x,n) +
 *          T(x,n+1).
 * https://oeis.org/A139146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabf, sign
 * Formula:   a(n) = coefficient of x^k in polynomial p(x,m) where p(x,m) = 2
 *            T(x, m/2) if m is even, T(x, (m−1)/2) + T(x, (m+1)/2) if m is
 *            odd, and the sequence lists all such coefficients in order of
 *            increasing m with the constant term first for each polynomial
 * Range:     i8 n <= 41, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chebyshev T_n(x) coefficients are generated once and reused.
   We pre‑compute T_n for n up to a fixed limit sufficient for n ≤ 10 000.  */
#include <stdlib.h>

static size_t const MAX_T_N = 300;   /* safe for n ≤ 10000  */

static seq_t **T_coeffs = NULL;  /* array of pointers to coefficient arrays */
static size_t *T_deg = NULL;     /* degree of each T_n */

/* Helper to initialise the T_n tables (called once) */
static void init_chebyshev(void)
{
    if (T_coeffs != NULL) return;     /* already initialised */

    T_coeffs = (seq_t**)malloc((MAX_T_N + 1) * sizeof(seq_t*));
    T_deg    = (size_t*)malloc((MAX_T_N + 1) * sizeof(size_t));
    if (!T_coeffs || !T_deg) return;   /* allocation failed – let caller see 0 */

    /* T_0 = 1 */
    T_coeffs[0] = (seq_t*)calloc(1, sizeof(seq_t));
    T_coeffs[0][0] = (seq_t)1;
    T_deg[0] = 0;

    /* T_1 = x  -> [0, 1] */
    T_coeffs[1] = (seq_t*)calloc(2, sizeof(seq_t));
    T_coeffs[1][0] = (seq_t)0;
    T_coeffs[1][1] = (seq_t)1;
    T_deg[1] = 1;

    for (size_t n = 2; n <= MAX_T_N; ++n) {
        size_t deg_prev = T_deg[n - 1];
        size_t deg_prevprev = T_deg[n - 2];

        /* allocate space for T_n, degree n */
        seq_t *newcoeff = (seq_t*)calloc(n + 1, sizeof(seq_t));

        /* 2*x * T_{n-1} : shift by one position and multiply by 2 */
        for (size_t k = 0; k <= deg_prev; ++k) {
            newcoeff[k + 1] += T_coeffs[n - 1][k] * (seq_t)2;
        }

        /* - T_{n-2} */
        for (size_t k = 0; k <= deg_prevprev; ++k) {
            newcoeff[k] -= T_coeffs[n - 2][k];
        }

        T_coeffs[n] = newcoeff;
        T_deg[n] = n;
    }
}

/* Retrieve coefficient of x^k in T_n(x). 0 if k > n. */
static inline seq_t coeff_T(size_t n, size_t k)
{
    if (k > T_deg[n]) return (seq_t)0;
    return T_coeffs[n][k];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    init_chebyshev();

    /* Find the polynomial index m and local coefficient index k.    */
    idx_t m = 0;
    idx_t cum = 0;                      /* cumulative coefficient count up to m-1 */
    while (1) {
        /* Length of polynomial m: even -> m/2+1, odd -> (m+3)/2 */
        idx_t len = (m & 1) ? ((m + 3) >> 1) : ((m >> 1) + 1);
        if (cum + len >= n) break;
        cum += len;
        ++m;
    }
    idx_t k_idx = n - cum - 1;          /* zero‑based index within this polynomial */

    seq_t result = (seq_t)0;

    if ((m & 1) == 0) {
        /* even: p = 2 * T_{m/2} */
        size_t tn = (size_t)(m >> 1);
        result = coeff_T(tn, (size_t)k_idx) * (seq_t)2;
    } else {
        /* odd: p = T_{(m-1)/2} + T_{(m+1)/2} */
        size_t tn = (size_t)((m - 1) >> 1);
        result = coeff_T(tn,     (size_t)k_idx) +
                 coeff_T(tn + 1, (size_t)k_idx);
    }

    return result;
}
