/*
 * A173590  T(n, k) = k^n*U(n, (1/k + k)/2) + (n + 1)^(k - 1)*U(k - 1, (1/(n +
 *          1) + n + 1)/2), where U(n,x) is the n-th Chebyshev polynomial of
 *          the second kind, square array read by antidiagonals (n >= 0, k >=
 *          1).
 * https://oeis.org/A173590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(n, k) with T(n, k) = k^n*U(n, (1/k + k)/2) + (n + 1)^(k
 *            - 1)*U(k - 1, (1/(n + 1) + n + 1)/2) where U is the Chebyshev
 *            polynomial of the second kind. a(n) is the nth term of the array
 *            read by antidiagonals (n ≥ 0, k ≥ 1). The coordinates (n, k) for
 *            index n are recovered by: s = floor((√(8...
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t N)
{
    /* Find diagonal parameters */
    idx_t s = ( ( (idx_t)floor_sqrt((seq_t)(8 * N + 1)) + 1 ) / 2 );
    idx_t t = s * (s - 1) / 2;
    idx_t m = N - t;
    idx_t n_val = s - 1 - m;   /* row index */
    idx_t k_val = s - n_val;   /* column index */

    /* First part of T(n,k) */
    seq_t term1;
    if (k_val == 1) {
        term1 = (seq_t)n_val + 1;           /* n+1 */
    } else {
        seq_t base  = (seq_t)k_val;
        seq_t exp   = (seq_t)(2 * n_val + 2);
        seq_t num   = big_pow(base, exp);
        num -= 1;
        seq_t denom = (seq_t)(k_val * k_val - 1);
        term1 = num / denom;
    }

    /* Second part of T(n,k) */
    seq_t term2;
    idx_t m_val = n_val + 1;
    if (m_val == 1) {
        term2 = (seq_t)k_val;                 /* limit as (n+1)→1 */
    } else {
        seq_t base2  = (seq_t)m_val;
        seq_t exp2   = (seq_t)(2 * k_val);
        seq_t num2   = big_pow(base2, exp2);
        num2 -= 1;
        seq_t denom2 = (seq_t)(m_val * m_val - 1);
        term2 = num2 / denom2;
    }

    return term1 + term2;
}
