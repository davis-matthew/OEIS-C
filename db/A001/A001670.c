/*
 * A001670  k appears k times (k even).
 * https://oeis.org/A001670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*round(sqrt(n)) = 2*floor(1/2 + sqrt(n)); with k =
 *            floor(sqrt(n)), a(n) = 2*(k+1) if n > k^2+k, else 2*k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001670_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001670_isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t k = a001670_isqrt_u64((uint64_t)n);
    if ((uint64_t)n > k * k + k) {
        k = k + 1;
    }
    return (seq_t)k * 2;
}
