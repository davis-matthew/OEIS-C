/*
 * A117402  Row sums of triangle A117401: a(n) = Sum_{k=0..n} 2^((n-k)*k) for
 *          n>=0.
 * https://oeis.org/A117402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^((n-k)*k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = k * (n - k);     /* exponent (n-k)*k */
        sum += pow2(exp);            /* 2^exp */
    }
    return sum;
}
