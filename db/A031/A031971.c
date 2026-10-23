/*
 * A031971  a(n) = Sum_{k=1..n} k^n.
 * https://oeis.org/A031971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = Sum_{k=1..n} k^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n < 1 the definition is not used, return 0 */
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += pow_uint(k, n);
    }
    return sum;
}
