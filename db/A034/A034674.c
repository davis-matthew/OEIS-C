/*
 * A034674  Sum of n-th powers of divisors of 128.
 * https://oeis.org/A034674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(8*n) - 1)/(2^n - 1) = ∑_{i=0}^{7} 2^(i*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* base = 2^n */
    seq_t base = seq_pow((seq_t)2, n);

    seq_t term = (seq_t)1;
    seq_t sum  = (seq_t)0;

    /* Sum the geometric progression of 8 terms */
    for (idx_t i = 0; i < 8; ++i)
    {
        sum += term;
        term = seq_mul(term, base);
    }
    return sum;
}
