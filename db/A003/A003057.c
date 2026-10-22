/*
 * A003057  n appears n - 1 times.
 * https://oeis.org/A003057
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that n-1 <= C(k,2), i.e. a(n) =
 *            ceiling((sqrt(8*n - 7) + 1)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003057_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003057_isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n - 1;   /* need smallest k with k*(k-1)/2 >= m */
    uint64_t k = a003057_isqrt_u64(2 * m);
    if (k < 2) k = 2;
    while (k * (k - 1) / 2 < m) k++;
    while (k > 2 && (k - 1) * (k - 2) / 2 >= m) k--;
    return (seq_t)k;
}
