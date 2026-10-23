/*
 * A038311  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*9^j.
 * https://oeis.org/A038311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   For n ≥ 0 let * i = ⌊(√(8 n + 1) – 1) / 2⌋ (row index) * j = n –
 *            i · (i + 1) / 2 (column index) * then a(n) = binomial(i, j) ·
 *            10^(i – j) · 9^j
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow_u8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_u8.h"
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t t = 1; t <= k; ++t) {
        res = res * (seq_t)(n - k + t) / (seq_t)t;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row index i */
    idx_t i = 0;
    while ((idx_t)(i + 1) * (i + 2) / 2 <= n) ++i;

    /* column offset j within the row */
    idx_t row_start = i * (i + 1) / 2;
    idx_t j = n - row_start;

    /* compute binomial(i,j) *
       10^(i-j) * 9^j  */
    seq_t bin = binomial(i, j);
    seq_t pow10 = pow_u8((seq_t)10, i - j);
    seq_t pow9  = pow_u8((seq_t)9,  j );

    return bin * pow10 * pow9;
}
