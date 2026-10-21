/*
 * A227706  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially as G(n) in the Darafsheh et al. reference.
 * https://oeis.org/A227706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(5184*n^2 - 10368*n + 18696) - 2^n*19314 + 1095
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t poly = (seq_t)5184 * n_seq * n_seq
               - (seq_t)10368 * n_seq
               + (seq_t)18696;

    seq_t pow4_n = pow2((idx_t)(2*n));  /* 4^n = 2^(2n) */
    seq_t pow2_n = pow2(n);             /* 2^n */

    seq_t term1 = pow4_n * poly;
    seq_t term2 = pow2_n * (seq_t)19314;

    return term1 - term2 + (seq_t)1095;
}
