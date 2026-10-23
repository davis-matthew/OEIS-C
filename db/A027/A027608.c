/*
 * A027608  Expansion of 1/((1-x)*(1-2*x)^4).
 * https://oeis.org/A027608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/3)*(n^2 + 3*n + 8)*2^n + 1, n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 20, u64 n <= 48, u128 n <= 109,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;               /* cast index to seq_t         */
    seq_t pow2n = pow2(n);                /* 2^n                          */

    /* compute (n*(n^2 + 3*n + 8)) / 3 * 2^n + 1   */
    seq_t tmp = n_seq * (n_seq * n_seq + 3 * n_seq + 8);
    tmp = tmp / 3;                        /* integer division, exact     */
    tmp = tmp * pow2n;
    return tmp + 1;
}
