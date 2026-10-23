/*
 * A137456  A triangular sequence of coefficients of a partition two types
 *          polynomials; of Chebyshev of the first kind polynomials (A053120)
 *          and Hermite polynomials (A060821): p(x,n) = T(x,n)*H(x,n).
 * https://oeis.org/A137456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, uned, sign
 * Formula:   a(n) is the nth coefficient of the triangular array of *
 *            coefficients of the product of Chebyshev polynomials T(x,k) and
 *            * Hermite polynomials H(x,k). Each row k has 2k+1 coefficients,
 *            * and the rows are concatenated for the sequence. The sequence *
 *            is indexed starting at 1
 * Range:     i8 n <= 18, i16 n <= 42, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stddef.h>

/* Maximum degree we expect to support for n up to ~10000. */
#define MAX_N  200
#define MAX_LEN (2*MAX_N+1)

/* Tables for T(x,k), H(x,k) and their product P(x,k) = T*H. */
static seq_t T[MAX_N+1][MAX_LEN+1];
static seq_t H[MAX_N+1][MAX_LEN+1];
static seq_t P[MAX_N+1][MAX_LEN+1];

/* Highest degree whose values have been computed. */
static idx_t max_pre = 0;

/* Precompute tables up to degree 'need'. */
static void precompute(idx_t need)
{
    /* Ensure base case (degree 0) is initialized. */
    if (max_pre == 0) {
        T[0][0] = 1;
        H[0][0] = 1;
        P[0][0] = 1;
    }

    if (need <= max_pre) return;

    /* Initialise degree 1 if needed. */
    if (need >= 1 && max_pre < 1) {
        T[1][0] = 0;  T[1][1] = 1;
        H[1][0] = 0;  H[1][1] = 2;
        for (idx_t k = 0; k <= 2; ++k) P[1][k] = 0;
        P[1][2] = 2;          /* product T_1*H_1 = 2x^2 */
        max_pre = 1;
    }

    /* Build higher degrees. */
    while (max_pre < need) {
        idx_t n   = max_pre;          /* current degree */
        idx_t np1 = n + 1;            /* next degree to compute */

        /* T_{n+1} = 2x T_n - T_{n-1} */
        for (idx_t k = 0; k <= np1; ++k) {
            seq_t val = 0;
            if (k >= 1 && k-1 <= n) val += (seq_t)2 * T[n][k-1];
            if (k <= n-1)           val -=        T[n-1][k];
            T[np1][k] = val;
        }

        /* H_{n+1} = 2x H_n - 2n H_{n-1} */
        for (idx_t k = 0; k <= np1; ++k) {
            seq_t val = 0;
            if (k >= 1 && k-1 <= n) val += (seq_t)2 * H[n][k-1];
            if (k <= n-1)           val -= (seq_t)(2 * n) * H[n-1][k];
            H[np1][k] = val;
        }

        /* Clear product row before convolution. */
        for (idx_t k = 0; k <= 2*np1; ++k) P[np1][k] = 0;

        /* Convolution: P_{n+1} = T_{n+1} * H_{n+1} */
        for (idx_t i = 0; i <= np1; ++i)
            for (idx_t j = 0; j <= np1; ++j)
                P[np1][i+j] += T[np1][i] * H[np1][j];

        ++max_pre;
    }
}

/* Return the nth term of the sequence, n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for 0, but safe */

    /* Find row degree d such that (d+1)^2 >= n. */
    idx_t d = (idx_t)ceil(sqrt((long double)n)) - 1;
    idx_t pos = n - d*d - 1;             /* 0‑based position within row */

    precompute(d);                      /* ensure tables up to degree d */

    return P[d][pos];
}
