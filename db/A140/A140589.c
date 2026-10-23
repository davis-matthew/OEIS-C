/*
 * A140589  Triangle A(k,n) = (-2)^k+2^n read by rows.
 * https://oeis.org/A140589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-2)^k + 2^j where k = floor((sqrt(8n+1)-1)/2) and j = n
 *            - k*(k+1)/2
 * Range:     i8 n <= 26, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t triangular(idx_t k)
{
    return k * (k + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the row index k such that triangular(k) <= n < triangular(k+1) */
    idx_t k = 0;
    while (triangular(k + 1) <= n)
        ++k;

    idx_t j = n - triangular(k);

    seq_t term_k = pow2(k);
    if (k & 1)
        term_k = -term_k;             /* (-2)^k = (-1)^k * 2^k */

    seq_t term_j = pow2(j);            /* 2^j */

    return term_k + term_j;
}
