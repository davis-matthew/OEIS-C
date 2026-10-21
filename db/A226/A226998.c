/*
 * A226998  The number of descents over all functions
 *          f:{1,2,...,n}->{1,2,...,n}.
 * https://oeis.org/A226998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-2)*n^n + n^{n-1})/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* n^n  */
    seq_t nn = big_pow((seq_t)n, (seq_t)n);

    /* (n-2)*n^n  */
    seq_t term1 = (seq_t)(n - 2) * nn;

    /* n^{n-1}  */
    seq_t term2 = big_pow((seq_t)n, (seq_t)(n - 1));

    /* Final result */
    return (term1 + term2) / (seq_t)2;
}
