/*
 * A060992  a(n) = Sum_{gcd(i,j) | 0 < i <= j < n and i+j = n}.
 * https://oeis.org/A060992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} gcd(n-i, i)
 * Range:     u8 n <= 89, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code implements the sum of gcds for each pair (i, n-i)
   with 1 ≤ i ≤ n/2. It uses a lightweight Euclidean algorithm
   and works with the integer types seq_t and idx_t supplied by the
   build environment. */

#include <stdint.h>
#include "../utils/local_gcd_uint64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n >> 1;            /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i) {
        uint64_t a_val = (uint64_t)(n - i);
        uint64_t b_val = (uint64_t)i;
        sum += local_gcd_uint64(a_val, b_val);
    }
    return sum;
}
