/*
 * A063772  a(k^2 + i) = k + a(i) for k >= 0 and 0 <= i <= k * 2; a(0) = 0.
 * https://oeis.org/A063772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of floor(sqrt(remaining)) while repeatedly
 *            subtracting the largest square from the remaining value,
 *            starting with remaining = n and a(0) = 0. This is equivalent to
 *            the recurrence a(k^2 + i) = k + a(i) with 0 ≤ i ≤ 2k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t rem = n;
    while (rem > 0) {
        idx_t k = _floor_isqrt_u64(rem);   /* largest k such that k^2 ≤ rem */
        sum += (seq_t)k;                   /* add k to the result */
        rem -= k * k;                      /* subtract the square k² */
    }
    return sum;
}
