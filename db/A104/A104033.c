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
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

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
            seq_t prod = binomial(N, K) * T[j];
            sum += prod;
        }
        if (m == c)
            T[m] = 1 - sum;
        else
            T[m] = -sum;
    }

    return T[r];
}
