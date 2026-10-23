/*
 * A106270  Inverse of number triangle A106268; triangle T(n,k), 0 <= k <= n.
 * https://oeis.org/A106270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the (row‑major) entry of the number triangle T(r,k) = 1
 *            if k == r -Catalan(r-k) otherwise. For n = r*(r+1)/2 + k with 0
 *            ≤ k ≤ r a(n) = (k == r) ? 1 : -Catalan(r-k)
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom, triangular_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/triangular_row.h"

seq_t a(idx_t n)
{
    /* find row r and column k */
    idx_t r = triangular_row(n);
    idx_t k = n - (idx_t)((__int128)r * (r + 1) / 2);
    /* if last column, value is 1 */
    if (k == r) return (seq_t)1;
    /* compute -Catalan(r-k) */
    idx_t m = r - k;                     /* 0 < m ≤ r */
    seq_t catalan = binom(2 * m, m);     /* C(2m, m) */
    catalan /= (seq_t)(m + 1);           /* divide by (m+1) */
    return -catalan;
}
