/*
 * A227699  The Wiener index of the nanostar dendrimer defined pictorially in
 *          the Ashrafi et al. reference.
 * https://oeis.org/A227699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8905 + 2^n*(10560*n + 21528) + 4^n*(11520*n + 3648)
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"  /* pow2(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    seq_t result = 8905;                         /* constant part */

    /* 2^n * (10560*n + 21528) */
    seq_t pow2n          = pow2(n);              /* 2^n                */
    seq_t coeff2n_part   = ((seq_t)10560 * n) + 21528;
    result += pow2n * coeff2n_part;

    /* 4^n * (11520*n + 3648)  --> 4^n = 2^(2n) */
    seq_t pow2_2n = pow2(2 * n);                  /* 2^(2n) = 4^n       */
    seq_t coeff4n_part = ((seq_t)11520 * n) + 3648;
    result += pow2_2n * coeff4n_part;

    return result;
}
