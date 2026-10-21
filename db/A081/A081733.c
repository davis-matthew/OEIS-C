/*
 * A081733  Triangle read by rows, T(n,k) = 2^(n-k)*[x^k] Euler_polynomial(n,
 *          x), for n >= 0, k >= 0.
 * https://oeis.org/A081733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binom(row,col) * P[row-col] , where row and col are
 *            obtained from n by triangular indexing (n = row(row+1)/2 + col,
 *            0 ≤ col ≤ row) and P[m] = 2^m * E_m(0) satisfies P[0] = 1, P[m]
 *            = - (1/2) * Σ_{i=0}^{m-1} binom(m,i) * P[i] * 2^{m-i}. The
 *            recurrence follows from the generating function 2/(...
 * Range:     i8 n <= 27, i16 n <= 55, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   binom, pow2, row_col_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/row_col_from_index.h"

/* dynamic array of P values */
static seq_t *P_array = NULL;
static idx_t P_len = 0;  /* number of valid entries in P_array */

/* ensure that P_array[0..m] are computed */
static void ensure_P(idx_t m)
{
    if (P_len > m) return;

    /* expand array if necessary */
    size_t new_len = (size_t)(m + 1);
    if (P_array == NULL || new_len > P_len) {
        P_array = (seq_t *)realloc(P_array, new_len * sizeof(seq_t));
    }

    for (idx_t i = P_len; i <= m; ++i) {
        if (i == 0) {
            P_array[0] = 1;  /* P[0] = 1 */
            continue;
        }
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            seq_t bin = binom(i, j);
            seq_t pw  = pow2(i - j);          /* 2^(i-j) */
            seq_t term = bin * P_array[j] * pw;
            sum += term;
        }
        /* divide by 2 – the sum is always even */
        P_array[i] = -(sum / 2);
    }
    P_len = m + 1;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    row_col_from_index(n, &row, &col);

    idx_t diff = row - col;       /* m = row - col */
    ensure_P(diff);

    seq_t bin = binom(row, col);
    seq_t val = bin * P_array[diff];
    return val;
}
