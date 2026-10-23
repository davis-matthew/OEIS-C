/*
 * A104033  Triangle, read by rows, equal to the matrix inverse of triangle
 *          A103327, where A103327(n,k) = binomial(2*n+1,2*k+1).
 * https://oeis.org/A104033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattening of the triangle T, where T(m,k) satisfies
 *            T(m,k) = δ_{m==k} - sum_{j=0}^{m-1} binom(2*m+1, 2*j+1) * T(j,k)
 *            (1) Here binom(N,K) is the binomial coefficient C(N,K)
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 27, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of.h"
#include "../utils/binom.h"

#define MAX_ROW 200   /* safe upper bound: for n < 10000, row index < 141 */

static seq_t T[MAX_ROW];

seq_t a(idx_t n)
{
    idx_t r = row_of(n);                     /* row index */
    idx_t base = r * (r + 1) / 2;
    idx_t c = n - base;                      /* column index */

    /* Ensure array is big enough for this row */
    if (r + 1 > (idx_t)MAX_ROW)
        return (seq_t)0;   /* should not happen for the intended limits */

    /* Initialize the array for this column */
    for (idx_t i = 0; i <= r; ++i)
        T[i] = 0;

    /* Compute T(m,c) for m = 0 .. r using recurrence (1) */
    for (idx_t m = 0; m <= r; ++m)
    {
        seq_t sum = 0;
        for (idx_t j = 0; j < m; ++j)
        {
            idx_t N = 2 * m + 1;
            idx_t K = 2 * j + 1;
            seq_t prod = binom(N, K) * T[j];
            sum += prod;
        }
        if (m == c)
            T[m] = 1 - sum;
        else
            T[m] = -sum;
    }

    return T[r];
}
