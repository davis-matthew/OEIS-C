/*
 * A179853  E.g.f. A(x) = Sum_{n>=0} a(n)*x^(3n)/(3n)!.
 * https://oeis.org/A179853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n-1) * (3*n)! / n! (OEIS A179853)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* (n+1)^(n-1) */
    seq_t pow = big_pow((seq_t)(n + 1), (seq_t)(n - 1));

    /* product of the integers from (n+1) to (3*n) inclusive */
    seq_t prod = (seq_t)1;
    for (idx_t k = n + 1; k <= 3 * n; ++k)
        prod = prod * (seq_t)k;

    return pow * prod;
}
