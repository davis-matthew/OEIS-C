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
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-03)
 * Helpers:   binomial, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* convert linear index n to (row,col) in the triangle */
static void row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    /* increase r until the next triangular number exceeds n */
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n) ++r;
    *row = r;
    __int128 tri = (__int128)r * (r + 1) / 2;
    *col = n - (idx_t)tri;
}

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
        P_array = mem_resize(seq_t, P_array, new_len);
    }

    for (idx_t i = P_len; i <= m; ++i) {
        if (i == 0) {
            P_array[0] = 1;  /* P[0] = 1 */
            continue;
        }
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            seq_t bin = binomial(i, j);
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

    seq_t bin = binomial(row, col);
    seq_t val = bin * P_array[diff];
    return val;
}
