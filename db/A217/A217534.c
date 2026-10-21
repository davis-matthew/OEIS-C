/*
 * A217534  a(n) = (n+3)^n - (3^n + 4^n + ... + (n+2)^n).
 * https://oeis.org/A217534
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)^n - Sum_{k=3..n+2} k^n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 2 by the OEIS offset; behaviour below 2 is irrelevant */
    seq_t total = seq_pow((seq_t)(n + 3), n);
    seq_t sum = 0;
    for (idx_t k = 3; k <= n + 2; ++k)
    {
        sum += seq_pow((seq_t)k, n);
    }
    return total - sum;
}
