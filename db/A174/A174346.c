/*
 * A174346  Triangle T(n, k) = (binomial(n-1, k-1)*binomial(n, k-1)/k) * (
 *          3^(k-1) if floor(n/2) >= k, otherwise 3^(n-k) ), read by rows.
 * https://oeis.org/A174346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = minimal integer with r*(r+1)/2 >= n *
 *            c = n - (r-1)*r/2 * T(r,c) = ( binomial(r-1,c-1) *
 *            binomial(r,c-1) / c ) * * 3^{ ( r/2 >= c ? c-1 : r-c ) } *
 *            (indexing starts at 1)
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 112, u64 n <= 364,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial_fu, pow3_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_fu.h"
#include "../utils/pow3_u.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for n==0, return 0 for safety */
    /* find row r */
    idx_t r = 1;
    idx_t cnt = 1;               /* number of terms up to row r: r*(r+1)/2 */
    while (cnt < n) {
        r++;
        cnt += r;
    }
    /* column c */
    idx_t prev_cnt = cnt - r;    /* number of terms before row r */
    idx_t c = n - prev_cnt;      /* 1 <= c <= r */

    /* compute binomial parts */
    seq_t bin1 = binomial_fu(r - 1, c - 1);
    seq_t bin2 = binomial_fu(r, c - 1);
    seq_t prod = bin1 * bin2;
    prod = prod / (seq_t)c;

    /* exponent of 3 */
    idx_t exp;
    if ((r / 2) >= c)
        exp = c - 1;
    else
        exp = r - c;

    seq_t pow3 = pow3_u(exp);
    return prod * pow3;
}
