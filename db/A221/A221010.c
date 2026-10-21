/*
 * A221010  The Wiener index of the nanostar dendrimer G[n], defined
 *          pictorially in the Nadjafi-Arani et al. reference.
 * https://oeis.org/A221010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -270 + 2^(n-1)*2055 + 4^(n-1)*(1323*n-3024)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute 2^(n-1) and 4^(n-1) */
    seq_t pow2 = seq_pow((seq_t)2, n - 1);
    seq_t pow4 = seq_pow((seq_t)4, n - 1);

    /* 2^(n-1) * 2055 */
    seq_t t1 = seq_mul(pow2, (seq_t)2055);

    /* 4^(n-1) * (1323*n) */
    seq_t mul1 = seq_mul((seq_t)n, (seq_t)1323);
    seq_t t2   = seq_mul(pow4, mul1);

    /* 4^(n-1) * 3024 */
    seq_t r1 = seq_mul(pow4, (seq_t)3024);

    /* combine the terms: t1 + t2 - 270 - r1 */
    seq_t result = t1 + t2;
    result -= (seq_t)270;
    result -= r1;

    return result;
}
