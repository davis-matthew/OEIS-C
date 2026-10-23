/*
 * A161789  a(n) is the largest integer k such that 2^k - 1 divides n.
 * https://oeis.org/A161789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest integer k such that (2^k - 1) divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* By definition a(1) = 1.  The loop below handles all n >= 1. */
    if (n == 0) return 0;              /* defensive, never needed */
    for (int k = 64; k >= 1; --k) {
        /* 2^k - 1 fits in uint64_t, except for k == 64 where the
           value is UINT64_MAX. */
        uint64_t divisor = (k == 64) ? UINT64_MAX : ((1ULL << k) - 1);
        if (n % divisor == 0) {
            return (seq_t)k;
        }
    }
    return 0;   /* unreachable for n >= 1 */
}
