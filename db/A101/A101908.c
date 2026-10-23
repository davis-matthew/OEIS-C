/*
 * A101908  Triangle read by rows: Characteristic polynomials of lower
 *          triangular Bell number matrix.
 * https://oeis.org/A101908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^{r-k} in the characteristic *
 *            polynomial of the r×r lower triangular Bell matrix, where r is
 *            the * row index of n and k = n - r(r+3)/2 - 1. Equivalently *
 *            a(n) = (-1)^k * e_k(B_1,B_2,…,B_r) with e_k the k‑th elementary
 *            * symmetric sum of the first r Bell numbers B_1,…...
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 33, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"


#define MAX_R 2000   /* sufficient for n up to ~10⁵ (r ≈ 450) */

seq_t a(idx_t n)
{
    /* Find the row r that contains index n and the 0‑based position pos in that row. */
    idx_t r = 1;
    idx_t cum = 2;                /* terms up to the current row r */
    while (cum < n) {
        ++r;
        cum += r + 1;
    }
    idx_t cum_before = cum - (r + 1);
    idx_t pos = n - cum_before - 1;

    /* ---------   compute Bell numbers B[1..r]   --------- */
    static seq_t B[MAX_R + 1];
    static idx_t B_computed = 0;

    if (B_computed == 0) {        /* initialise */
        B[0] = (seq_t)1;          /* B₀ */
        B[1] = (seq_t)1;          /* B₁ = 1 */
        B_computed = 1;
    }

    if (r > B_computed) {
        for (idx_t i = B_computed; i < r; ++i) {
            seq_t sum = 0;
            idx_t N = i;          /* compute B_{i+1} */
            for (idx_t k = 0; k <= N; ++k) {
                sum += binom(N, k) * B[k];
            }
            B[i + 1] = sum;
        }
        B_computed = r;
    }

    /* ---------  elementary symmetric sums of B[1..r]  --------- */
    seq_t e[MAX_R + 1];
    for (idx_t j = 0; j <= r; ++j) e[j] = 0;
    e[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        seq_t λ = B[i];
        for (idx_t j = i; j >= 1; --j) {
            e[j] = e[j] + λ * e[j - 1];
        }
    }

    seq_t coeff = e[pos];
    if (pos & 1) coeff = -coeff;   /* apply sign (-1)^pos */

    return coeff;
}
