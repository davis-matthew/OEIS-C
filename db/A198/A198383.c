/*
 * A198383  a(n) = Sum_{k=1..n} 2^(n mod k).
 * https://oeis.org/A198383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^(n mod k)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;
        sum += pow2(r);
    }
    return sum;
}
