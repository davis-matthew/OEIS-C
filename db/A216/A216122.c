/*
 * A216122  The Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Ashrafi et al. reference.
 * https://oeis.org/A216122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -55182 + 181020*2^n - 125640*4^n - 42000*n*2^n +
 *            93600*n*4^n
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 21, u128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2_n  = pow2(n);          /* 2^n   */
    seq_t pow4_n  = pow2(2 * n);      /* 4^n = 2^(2n) */

    seq_t term1 = 181020 * pow2_n;                    /* 181020*2^n   */
    seq_t term2 = 93600 * ((seq_t)n) * pow4_n;        /* 93600*n*4^n   */
    seq_t term3 = 125640 * pow4_n;                    /* 125640*4^n   */
    seq_t term4 = 42000 * ((seq_t)n) * pow2_n;        /* 42000*n*2^n   */

    seq_t res = term1 + term2;
    res -= term3;
    res -= term4;
    res -= 55182;
    return res;
}
