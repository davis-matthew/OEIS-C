/*
 * A221005  The hyper-Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Wang-Hua reference.
 * https://oeis.org/A221005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2 + 2^n*(1404*n^2 + 1764*n - 3115) + 4^n*(1296*n^2 -
 *            4536*n + 7914)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n and 4^n */
    seq_t pow2_n = pow2(n);
    seq_t pow4_n = pow2(2 * n);        /* 4^n = 2^(2n) */

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq; /* n^2 */

    /* Term coming from 2^n part: 2^n * (1404*n^2 + 1764*n - 3115) */
    seq_t term1 = pow2_n * (seq_t)1404 * n_sq;      /* 2^n * 1404*n^2 */
    term1 += pow2_n * (seq_t)1764 * n_seq;          /* 2^n * 1764*n   */
    term1 -= pow2_n * (seq_t)3115;                  /* 2^n * 3115     */

    /* Term coming from 4^n part: 4^n * (1296*n^2 - 4536*n + 7914) */
    seq_t term2 = pow4_n * (seq_t)1296 * n_sq;      /* 4^n * 1296*n^2 */
    term2 += pow4_n * (seq_t)7914;                  /* 4^n * 7914     */
    term2 -= pow4_n * (seq_t)4536 * n_seq;          /* 4^n * 4536*n   */

    /* Combine terms and subtract the final constant 2 */
    seq_t result = term1 + term2;
    result -= (seq_t)2;

    return result;
}
