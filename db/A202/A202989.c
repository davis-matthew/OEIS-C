/*
 * A202989  E.g.f: Sum_{n>=0} 3^(n^2) * exp(3^n*x) * x^n/n!.
 * https://oeis.org/A202989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n + 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    /* (3^n + 1) */
    pow3 += (seq_t)1;
    /* (3^n + 1)^n */
    return big_pow(pow3, (seq_t)n);
}
