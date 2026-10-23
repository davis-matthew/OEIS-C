/*
 * A193131  Numbers of spanning trees of the complete tripartite graphs
 *          K_{n,n,n}.
 * https://oeis.org/A193131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*8^(n-1)*n^(3*n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of spanning trees of the complete tripartite graph K_{n,n,n}. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the OEIS offset; no special handling below 1. */
    seq_t n_seq = (seq_t)n;

    /* Compute 8^(n-1).  n-1 fits in idx_t, cast to seq_t for the exponent. */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)(n - 1));

    /* Compute n^(3*n-2).  Compute the exponent as 3*n-2, cast to seq_t. */
    seq_t exp_n = (seq_t)(3 * n_seq - 2);
    seq_t pow_n = big_pow(n_seq, exp_n);

    /* Final result: 3 * 8^(n-1) * n^(3*n-2). */
    seq_t result = (seq_t)3 * pow8 * pow_n;
    return result;
}
