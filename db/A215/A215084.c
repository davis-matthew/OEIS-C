/*
 * A215084  a(n) = sum of the sums of the k first n-th powers.
 * https://oeis.org/A215084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} (n+1-j) * j^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;            /* a(0) = 0 as in the OEIS list */

    seq_t N = (seq_t)n;
    seq_t total = (seq_t)0;

    for (idx_t j = (idx_t)1; j <= n; ++j) {
        seq_t base  = (seq_t)j;
        seq_t pj    = big_pow(base, N);           /* j^n */
        seq_t coeff = N + (seq_t)1 - (seq_t)j;    /* n+1-j */
        total += coeff * pj;
    }
    return total;
}
