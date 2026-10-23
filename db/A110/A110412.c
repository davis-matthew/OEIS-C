/*
 * A110412  Sum_{d<n is a divisor of n} tau(n-d).
 * https://oeis.org/A110412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d<n} τ(n-d) (sum of divisor counts of n-d over
 *            all proper divisors d)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static inline seq_t count_divisors_uint64(uint64_t m) {
    seq_t c = 0;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            ++c;
            if (i != m / i) ++c;
        }
    }
    return c;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* defined only for n >= 1 */
    seq_t sum = 0;
    uint64_t nn = (uint64_t)n;
    for (uint64_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            uint64_t d1 = i;
            if (d1 < nn) {
                uint64_t nd = nn - d1;
                sum += count_divisors_uint64(nd);
            }
            uint64_t d2 = nn / i;
            if (d2 != d1 && d2 < nn) {
                uint64_t nd = nn - d2;
                sum += count_divisors_uint64(nd);
            }
        }
    }
    return sum;
}
