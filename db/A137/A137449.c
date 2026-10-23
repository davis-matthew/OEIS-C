/*
 * A137449  A triangular sequence based on concepts of operations on existing
 *          sequences: in this case the H(x,n) ( A060821) traditional Hermite
 *          is differentiated twice : p(x,n)=-x^2*H''(x,n)+H(x,n).
 * https://oeis.org/A137449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   a(n) = coefficients of p(x, n-1) where * p(x, m) = -x^2 * H''(x,
 *            m) + H(x, m), H_n(x) = 2x H_{n-1}(x) - 2(n-1) H_{n-2}(x) * with
 *            H_0(x) = 1, H_1(x) = 2x. * The first two rows are fixed: a(1) =
 *            a(2) = a(3) = 1
 * Range:     i8 n <= 14, i16 n <= 40, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_R 200  /* enough for n up to ~20000, r <= 200 (T_200 = 20100) */

static seq_t hermite[MAX_R + 1][MAX_R + 1];
static bool hermite_built = false;

/* Build all Hermite coefficients up to index MAX_R. */
static void build_hermite(void)
{
    if (hermite_built)
        return;

    /* H_0 = 1 */
    hermite[0][0] = (seq_t)1;

    /* H_1 = 2x */
    hermite[1][0] = (seq_t)0;
    hermite[1][1] = (seq_t)2;

    for (idx_t n = 2; n <= MAX_R; ++n) {
        seq_t n1 = (seq_t)(n - 1);
        for (idx_t e = 0; e <= n; ++e) {
            seq_t term1 = 0, term2 = 0;
            if (e >= 1) {
                term1 = (seq_t)2 * hermite[n - 1][e - 1];
            }
            if (e <= n - 2) {
                term2 = (seq_t)2 * n1 * hermite[n - 2][e];
            }
            hermite[n][e] = term1 - term2;
        }
    }

    hermite_built = true;
}

/* Return the n-th term of sequence A137449. */
seq_t a(idx_t n)
{
    /* Base values */
    if (n == 1 || n == 2 || n == 3)
        return (seq_t)1;

    /* Find the row r such that r(r+1)/2 >= n > (r-1)r/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t prevT = (r - 1) * r / 2;        /* first index of this row (1‑based) */
    idx_t k    = n - prevT;               /* column in the row (1‑based) */
    idx_t e    = k - 1;                   /* degree of the coefficient (0‑based) */
    idx_t nRow = r - 1;                   /* index of the Hermite polynomial */

    /* Ensure Hermite coefficients are available. */
    build_hermite();

    seq_t coeff = hermite[nRow][e];               /* H_{nRow}[e] */
    seq_t scalar = 1 - (seq_t)e * (seq_t)(e - 1); /* 1 - e*(e-1) */
    return coeff * scalar;
}
