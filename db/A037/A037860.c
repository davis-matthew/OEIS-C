/*
 * A037860  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*10^i:
 *          i=0,1,...,m} is base 10 representation of n.
 * https://oeis.org/A037860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} [d_i > d_{i-1}] * (d_i - d_{i-1}), where the
 *            decimal digits of n are d_0, d_1, …, d_m (d_0 least significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 10ULL) return 0;
    uint8_t prev = (uint8_t)(n % 10ULL);
    n /= 10ULL;
    while (n > 0) {
        uint8_t curr = (uint8_t)(n % 10ULL);
        if (curr > prev) {
            sum += (seq_t)(curr - prev);
        }
        prev = curr;
        n /= 10ULL;
    }
    return sum;
}
