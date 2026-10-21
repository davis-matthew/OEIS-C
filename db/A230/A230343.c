/*
 * A230343  Number of permutations of [2n+3] in which the longest increasing
 *          run has length n+3.
 * https://oeis.org/A230343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+8*n^2+20*n+19)*(2*n+3)!/(n+5)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* polynomial part: n^3 + 8*n^2 + 20*n + 19 */
    seq_t n_seq = (seq_t)n;
    seq_t poly = n_seq*n_seq*n_seq + (seq_t)8*n_seq*n_seq +
                 (seq_t)20*n_seq + (seq_t)19;

    if (n == 1)            /* special case where (2n+3)! < (n+5)!  */
        return poly / (seq_t)6;        /* 48/6 = 8 */

    /* compute (2n+3)!/(n+5)! as product of integers from n+6 to 2n+3 */
    seq_t prod = (seq_t)1;
    idx_t start = n + 6;
    idx_t end   = 2*n + 3;          /* may be < start when n == 2, loop skips */

    for (idx_t i = start; i <= end; ++i)
        prod = mul_small(prod, (unsigned int)i);

    return prod * poly;
}
