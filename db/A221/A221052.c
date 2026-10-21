/*
 * A221052  The Wiener index of the nanostar dendrimer NSC_5C_6[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference and in the M.
 *          Rostami et al. reference.
 * https://oeis.org/A221052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -14237 + 46170*2^n - 32004*4^n + 12960*n*4^n + 72*n*2^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 22, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute the n-th term of OEIS A221052.
 * The sequence is defined for n >= 1.
 */
seq_t a(idx_t n)
{
    /* compute powers of 2 and 4 as big integers */
    seq_t pow2_n = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4_n = big_pow((seq_t)4, (seq_t)n);

    /* 46170 * 2^n */
    seq_t result = 46170 * pow2_n;

    /* 72 * n * 2^n */
    seq_t tmp = (seq_t)n * 72;
    tmp = tmp * pow2_n;
    result += tmp;

    /* 12960 * n * 4^n */
    tmp = (seq_t)n * 12960;
    tmp = tmp * pow4_n;
    result += tmp;

    /* subtract (32004 * 4^n + 14237) */
    seq_t sub = 32004 * pow4_n;
    sub += 14237;
    result -= sub;

    return result;
}
