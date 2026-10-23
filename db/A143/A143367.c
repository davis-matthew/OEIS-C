/*
 * A143367  The Szeged index of third type dendrimer nanostar which has grown
 *          n stages.
 * https://oeis.org/A143367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3636*2^n + 1324 + 720*n*4^n + 1560*n*2^n - 1296*4^n
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 2^n and 4^n (4^n is simply (2^n)^2). */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4 = pow2 * pow2;   /* equivalently 4^n */

    /* Individual terms from the closed‑form expression. */
    seq_t term1 = (seq_t)3636 * pow2;
    seq_t term2 = (seq_t)1560 * (seq_t)n * pow2;
    seq_t term3 = (seq_t)720 * (seq_t)n * pow4;
    seq_t term4 = (seq_t)1324 - (seq_t)1296 * pow4;

    return term1 + term2 + term3 + term4;
}
