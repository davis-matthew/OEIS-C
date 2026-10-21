/*
 * A057052  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057052(n) = i(n^3).
 * https://oeis.org/A057052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(n^3) = n^3 - ((d-1)d)/2, where * d =
 *            ceil((sqrt(8n^3+1)-1)/2) is the diagonal containing n^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 31, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t k     = nseq * nseq * nseq;           /* k = n^3 */

    seq_t t     = (k << 3) + 1;                 /* 8*k + 1 */
    seq_t s     = floor_isqrt_seq(t);           /* floor sqrt(8k+1) */

    seq_t d     = (s - 1) / 2;                  /* tentative diagonal */

    seq_t Td    = d * (d + 1) / 2;              /* triangular number T_d */
    if (Td < k)
        d = d + 1;                              /* correct to next diagonal */

    seq_t Tprev = (d - 1) * d / 2;              /* T_{d-1} */
    return k - Tprev;                           /* i(k) */
}
