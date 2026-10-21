/*
 * A233757  Triangle read by rows: T(n,k) = (2^n-1)*2^(k-1), for n >= 1 and
 *          1<=k<=n.
 * https://oeis.org/A233757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{i=1}^{r(n)} (2^i - 1) * 2^{k(n)-1} where r(n) is the
 *            unique n such that n <= r(r+1)/2 and k(n) = n - r(r-1)/2
 *            (1-indexed). This implements the OEIS definition of A233757
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 152, u64 n <= 560,
 *            u128 n <= 2144, big any n
 * Helpers:   floor_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t n64 = (seq_t)n;

    /* Constants in seq_t form */
    const seq_t one = (seq_t)1;
    const seq_t two = (seq_t)2;
    const seq_t eight = (seq_t)8;

    /* Solve r(r+1)/2 >= n  -> r = ceil( (-1 + sqrt(1+8n)) / 2 ) */
    seq_t d = floor_sqrt(one + eight * n64);
    idx_t r = (idx_t)((d - one) / two);
    /* Check if the triangular number is still less than n */
    if ( ((seq_t)r * ((seq_t)r + one)) / two < n64 ) r++;

    /* Index within row r: k = n - (r-1)*r/2   */
    seq_t prior = ((seq_t)(r - 1) * (seq_t)r) / two;
    idx_t k = (idx_t)(n64 - prior);               /* 1 <= k <= r */

    /* Compute (2^r - 1) * 2^(k-1) */
    seq_t val1 = pow_uint(2ULL, r) - one;
    seq_t val2 = pow_uint(2ULL, (idx_t)(k - 1));
    return val1 * val2;
}
