/*
 * A224423  The Wiener index of the Micelle-like chiral dendrimer G[n] defined
 *          pictorially in the Hassan Yousefi-Azari et al. reference.
 * https://oeis.org/A224423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1189 + 15616*2^n + 7680*4^n + 12288*n*4^n + 4224*n*2^n
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

/* Compute a(n) for the OEIS sequence A224423 */
seq_t a(idx_t n)
{
    /* 2^n and 4^n in the target type */
    seq_t two_n   = pow2(n);
    seq_t four_n  = pow4(n);
    seq_t n_seq   = n;                 /* cast index to seq_t */
    seq_t result  = 1189;              /* constant term */

    result += (seq_t)15616 * two_n;
    result += (seq_t)7680  * four_n;
    result += (seq_t)12288 * n_seq * four_n;
    result += (seq_t)4224  * n_seq * two_n;

    return result;
}
