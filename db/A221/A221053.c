/*
 * A221053  The hyper-Wiener index of the nanostar dendrimer NSC_5C_6[n],
 *          defined pictorially in the A. R. Ashrafi et al. reference and in
 *          the M. Rostami et al. reference.
 * https://oeis.org/A221053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 204780 - 1311272*2^n + 1106480*4^n + 360*2^n*n^2 +
 *            129600*4^n*n^2 + 7852*2^n*n - 633600*4^n*n
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 19, u128 n <= 49,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2n = seq_pow((seq_t)2, n);
    seq_t pow4n = seq_pow((seq_t)4, n);
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;

    seq_t result = (seq_t)204780;
    result -= (seq_t)1311272 * pow2n;
    result += (seq_t)1106480 * pow4n;
    result += (seq_t)360 * pow2n * n_sq;
    result += (seq_t)129600 * pow4n * n_sq;
    result += (seq_t)7852 * pow2n * n_seq;
    result -= (seq_t)633600 * pow4n * n_seq;

    return result;
}
