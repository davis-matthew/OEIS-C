/*
 * A091135  Number of Dyck paths of semilength n+4, having exactly two long
 *          ascents (i.e., ascents of length at least two).
 * https://oeis.org/A091135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 9*n + 20)/2 + 2^(n+1)*(n^2 + 3*n - 4)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the only negative‑term corner case */
    if (n == 0)
        return (seq_t)2;

    seq_t sn      = (seq_t)n;           /* use seq_t for all arithmetic */
    seq_t n_sq    = sn * sn;            /* n^2 */

    /* first part (n^2 + 9*n + 20)/2 */
    seq_t term1 = (n_sq + (seq_t)9 * sn + (seq_t)20) / (seq_t)2;

    /* second part 2^(n+1) * (n^2 + 3*n - 4)  */
    seq_t mul    = n_sq + (seq_t)3 * sn - (seq_t)4;   /* non‑negative for n ≥ 1 */
    seq_t pow    = pow2((idx_t)(n + 1));              /* 2^(n+1) */
    seq_t term2  = pow * mul;

    return term1 + term2;
}
