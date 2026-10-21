/*
 * A090215  A generalization of triangles A071951 (Legendre-Stirling) and
 *          A089504.
 * https://oeis.org/A090215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Triangle a(n,m) = coefficient of x^(n-m) in 1 / ∏_{r=1..m} (1 -
 *            fallfac(r+3,4) * x), reduced to a( n ) = a( row, col ) with row,
 *            col defined below
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* g(x) = 1 / (1 - f x) expands as ∑ f^k x^k.  */

/* Maximum k needed for the problem (for n up to ~10000) is < 140. */
#define MAXK 200

static void emit_a_row_col(idx_t row, idx_t col, seq_t *result)
/* Computes a(row, col) as the coefficient of x^(row-col) in
   the product ∏_{r=1..col} 1/(1 - fallfac(r+3,4)x). */
{
    idx_t k = row - col;                /* exponent of x */
    seq_t coeff[MAXK];
    seq_t new_coeff[MAXK];
    seq_t fpow[MAXK];

    /* initialise coeff[0]=1, others 0 */
    for (idx_t i = 0; i <= k; ++i)
        coeff[i] = (i == 0) ? 1 : 0;

    for (idx_t r = 1; r <= col; ++r) {
        seq_t f = (seq_t)((r + 3) * (r + 2) * (r + 1) * r); /* fallfac(r+3,4) */
        fpow[0] = 1;
        for (idx_t t = 1; t <= k; ++t)
            fpow[t] = fpow[t - 1] * f;

        for (idx_t i = 0; i <= k; ++i) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= i; ++j)
                sum += coeff[j] * fpow[i - j];
            new_coeff[i] = sum;
        }
        for (idx_t i = 0; i <= k; ++i)
            coeff[i] = new_coeff[i];
    }
    *result = coeff[k];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, ignore 0 */

    /* Compute row r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt((long double)8 * n + 1) - 1) / 2);
    if (r * (r + 1) / 2 < n)
        ++r;

    /* Compute column c (1-indexed) */
    idx_t prev = r * (r - 1) / 2;
    idx_t c = n - prev;

    /* Now compute the entry a(row, col) */
    seq_t res;
    emit_a_row_col(r, c, &res);
    return res;
}
