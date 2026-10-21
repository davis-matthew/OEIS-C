/*
 * A224452  The Wiener index of the nanostar dendrimer NS_2[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 502 + 1280*n*4^n + 1120*n*2^n + 2000*2^n - 2304*4^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t two_n  = pow2(n);   /* 2^n */
    seq_t four_n = pow4(n);   /* 4^n = (2^n)^2 */

    seq_t n_seq = (seq_t)n;

    seq_t result =
        (seq_t)502
        + (seq_t)1280 * n_seq * four_n
        + (seq_t)1120 * n_seq * two_n
        + (seq_t)2000 * two_n
        - (seq_t)2304 * four_n;

    return result;
}
