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
 * Validated: standard, pass, 101 terms through n = 8541 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t rem = n;
    while (rem > 0) {
        idx_t k = isqrt(rem);   /* largest k such that k^2 ≤ rem */
        sum += (seq_t)k;                   /* add k to the result */
        rem -= k * k;                      /* subtract the square k² */
    }
    return sum;
}
