/*
 * A221050  The Wiener index of the nanostar dendrimer D_1[n], defined
 *          pictorially in the Eslahchi et al. reference.
 * https://oeis.org/A221050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28 + 2^n*(448*n+1554) + 4^n*(784*n-1540)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"
#include <stdint.h>

/* Computes the n-th term of OEIS sequence A221050. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t p2 = pow2(n);          /* 2^n   */
    seq_t p4 = pow4(n);          /* 4^n   */

    /* Positive components */
    seq_t pos1 = p2 * ((seq_t)448 * n_seq);   /* 2^n * 448 * n */
    seq_t pos2 = p2 * (seq_t)1554;            /* 2^n * 1554   */
    seq_t pos3 = p4 * ((seq_t)784 * n_seq);   /* 4^n * 784 * n */

    seq_t big = (seq_t)28 + pos1 + pos2 + pos3;   /* sum of positives */

    /* Subtractive component */
    seq_t sub = p4 * (seq_t)1540;               /* 4^n * 1540   */

    return big - sub;
}
