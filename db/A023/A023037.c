/*
 * A023037  a(n) = n^0 + n^1 + ... + n^(n-1), or a(n) = (n^n-1)/(n-1) with
 *          a(0)=0; a(1)=1.
 * https://oeis.org/A023037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n - 1) / (n - 1), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t base = (seq_t)n;
    seq_t pow_n = seq_pow(base, n);          /* base^n */
    seq_t numerator = pow_n - (seq_t)1;      /* base^n - 1 */
    seq_t denominator = base - (seq_t)1;     /* base - 1 */
    return numerator / denominator;
}
