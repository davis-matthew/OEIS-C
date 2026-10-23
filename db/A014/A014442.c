/*
 * A014442  Largest prime factor of n^2 + 1.
 * https://oeis.org/A014442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of n^2 + 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n) {
    /* OEIS offset is 1, so for n == 0 the value is undefined.
       Returning 0 here is harmless since the function is never
       called with n == 0 by the test harness.                    */
    if (n == 0) return 0;

    /* Compute n^2 + 1 using seq_t to stay within the type limits.
       For all practical n (≤ 10⁴) the result fits in 64 bits.   */
    seq_t m = (seq_t)n * (seq_t)n + 1;

    /* Now find the largest prime factor of m as an unsigned 64‑bit
       integer.  The helper function works on uint64_t and is fast
       enough for the expected size.                            */
    uint64_t val = (uint64_t)m;
    uint64_t gfp = largest_prime_factor(val);

    return (seq_t)gfp;
}
