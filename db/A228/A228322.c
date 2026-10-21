/*
 * A228322  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to the hypercube graph Q(n) (n>=1).
 * https://oeis.org/A228322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^(2*n) - 2^(n-2)*(4 + 12*n + n^2 + n^3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n < 1 */
    if (n == 1) return (seq_t)15;   /* base case */

    seq_t n_t = (seq_t)n;
    seq_t pow2_2n      = big_pow((seq_t)2, (seq_t)(2 * n));
    seq_t pow2_n_minus2 = big_pow((seq_t)2, (seq_t)(n - 2));

    seq_t n2 = n_t * n_t;            /* n^2 */
    seq_t n3 = n2 * n_t;             /* n^3 */
    seq_t sum = (seq_t)4 + (seq_t)12 * n_t + n2 + n3;

    seq_t term1 = (seq_t)6 * pow2_2n;
    seq_t term2 = pow2_n_minus2 * sum;

    return term1 - term2;
}
