/*
 * A006546  Number of elements (a b, c d) in SL(2,Z) with trace n and 0 <= a
 *          <= {b, c} <= d.
 * https://oeis.org/A006546
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{a=1..floor(n/2)} #{(b,c) : b*c = a*(n-a) - 1, a <= b
 *            <= n-a, a <= c <= n-a} (direct search over divisors)
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    for (uint64_t x = 1; 2 * x <= N; x++) {
        uint64_t d = N - x;
        uint64_t p = x * d - 1;   /* x >= 1, d >= x >= 1, so x*d >= 1 */
        if (p == 0) continue;
        for (uint64_t b = x; b * b <= p; b++) {
            if (p % b == 0) {
                uint64_t c = p / b;   /* c >= b >= x */
                if (c <= d) {
                    count += (b == c) ? 1 : 2;
                }
            }
        }
    }
    return (seq_t)count;
}
