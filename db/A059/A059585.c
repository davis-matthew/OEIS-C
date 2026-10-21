/*
 * A059585  Number of labeled 3-node T_0-hypergraphs with n hyperedges (empty
 *          hyperedges and multiple hyperedges included).
 * https://oeis.org/A059585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n + 7, n) - 3*binomial(n + 3, n) + 2*binomial(n
 *            + 1, n)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t t1 = binom(n + 7, n);     /* binomial(n+7, n)   */
    seq_t t2 = binom(n + 3, n);     /* binomial(n+3, n)   */
    seq_t t3 = binom(n + 1, n);     /* binomial(n+1, n)   */
    return t1 - 3 * t2 + 2 * t3;
}
