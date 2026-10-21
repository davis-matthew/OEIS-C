/*
 * A216123  The hyper-Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Ashrafi et al. reference.
 * https://oeis.org/A216123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 685719 - 2949654*2^n + 2264412*4^n + 495780*n*2^n -
 *            1584720*n*4^n - 136500*n^2*2^n + 608400*n^2*4^n
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; behavior for n = 0 is unused */
    if (n == 0) {
        return 0;
    }

    /* compute powers of two */
    seq_t two_n  = pow2(n);          /* 2^n  */
    seq_t four_n = pow2(2 * n);      /* 4^n  */

    /* n^2 as seq_t */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    seq_t result = (seq_t)685719;                 /* constant term */
    result -= (seq_t)2949654 * two_n;             /* -2949654*2^n */
    result += (seq_t)2264412 * four_n;            /* +2264412*4^n */
    result += (seq_t)495780 * (seq_t)n * two_n;   /* +495780*n*2^n */
    result -= (seq_t)1584720 * (seq_t)n * four_n; /* -1584720*n*4^n */
    result -= (seq_t)136500 * n_sq * two_n;       /* -136500*n^2*2^n */
    result += (seq_t)608400 * n_sq * four_n;      /* +608400*n^2*4^n */

    return result;
}
