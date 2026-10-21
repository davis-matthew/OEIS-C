/*
 * A079045  Coefficients of the polynomials in the numerator of the generating
 *          function f(x)=(x-x^2)/(x^3-2x^2-2x+1) for F(n)^2, (where F(n) is
 *          the Fibonacci sequence) and its successive derivatives starting
 *          with the constant.
 * https://oeis.org/A079045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) are the coefficients of the numerator polynomials of the
 *            successive * derivatives of * f(x) = (x - x^2) / (x^3 - 2x^2 -
 *            2x + 1) * (the generating function for the squares of the
 *            Fibonacci numbers). * The recurrence for the numerators is *
 *            N₍d₊₁₎(x) = N₍d₎'(x) * Q(x) - (d+1) * N₍d₎(x) * Q'(x)...
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_D 500                    /* enough for n up to ~10000 */
#define Q_DEG 4                      /* degree of Q(x) + 1 */
#define QP_DEG 3                     /* degree of Q'(x) + 1 */
#define POLY_LEN(d) (2 * (d) + 3)    /* length of N_d(x) */

/* static table of numerators N_d(x), d = 0 .. MAX_D */
static seq_t polys[MAX_D][POLY_LEN(MAX_D)];
static int polys_count = 0;            /* number of polynomials that have been computed */

/* coefficient arrays for Q(x) and its derivative Q'(x) */
static const seq_t Q[Q_DEG]   = { 1, -2, -2, 1 };   /* Q(x) = 1 - 2x - 2x² + x³   */
static const seq_t QP[QP_DEG] = { -2, -4, 3 };     /* Q'(x) = -2 - 4x + 3x²      */

/* compute the next numerator polynomial N_{d+1} from the current highest
 * polynomial N_d, where d = polys_count - 1.  The result is stored in
 * polys[d+1] and polys_count is incremented.  All intermediate work
 * uses arrays large enough for the maximum possible degree. */
static void compute_next_polynomial(void)
{
    int d = polys_count - 1;            /* current highest computed index */
    int len_cur  = POLY_LEN(d);
    int len_next = POLY_LEN(d + 1);

    /* NDprime: derivative of N_d, length len_cur-1 */
    seq_t NDprime[POLY_LEN(MAX_D)];
    for (int i = 0; i < POLY_LEN(MAX_D); ++i) NDprime[i] = 0;
    for (int k = 1; k < len_cur; ++k)
        NDprime[k - 1] = (seq_t)k * polys[d][k];

    /* next: product of NDprime with Q minus (d+1) * N_d * Q' */
    seq_t next[POLY_LEN(MAX_D)];
    for (int i = 0; i < POLY_LEN(MAX_D); ++i) next[i] = 0;

    /* term1 = NDprime * Q */
    for (int i = 0; i < len_cur - 1; ++i)
        for (int j = 0; j < Q_DEG; ++j)
            next[i + j] += NDprime[i] * Q[j];

    /* term2 = (d+1) * N_d * Q' */
    seq_t mult = (seq_t)(d + 1);
    for (int i = 0; i < len_cur; ++i)
        for (int j = 0; j < QP_DEG; ++j)
            next[i + j] -= mult * polys[d][i] * QP[j];

    /* copy the computed coefficients */
    for (int k = 0; k < len_next; ++k)
        polys[d + 1][k] = next[k];

    polys_count++;                      /* we now have polynomials up to d+1 */
}

/* ensure that all numerators up to degree d are computed */
static void ensure_polys_upto(int d)
{
    /* initialisation of N_0 if needed */
    if (polys_count == 0) {
        polys[0][0] = 0;   /* x⁰ coefficient */
        polys[0][1] = 1;   /* x¹ coefficient */
        polys[0][2] = -1;  /* x² coefficient */
        polys_count = 1;
    }
    while (polys_count <= d)
        compute_next_polynomial();
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    /* find the smallest d with (d+1)(d+3) > n  (total coefficients up to N_d) */
    int d = 0;
    while ((idx_t)(d + 1) * (d + 3) <= n)
        ++d;

    ensure_polys_upto(d);

    /* offset within the polynomial N_d */
    idx_t prev_total = (idx_t)d * (d + 2);          /* sum of lengths of N_0 .. N_(d-1) */
    idx_t offset = n - prev_total;                 /* index in N_d */

    return polys[d][(int)offset];
}
