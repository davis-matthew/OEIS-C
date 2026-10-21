/*
 * A060866  Sum of (d+d') over all unordered pairs (d,d') with d*d' = n.
 * https://oeis.org/A060866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + sqrt(n) if n is a perfect square (otherwise
 *            sigma(n))
 * Range:     u8 n <= 107, u16 n <= 9604, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t N = (uint64_t)n;
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = N / i;
            if (d1 == d2) {
                sum += (seq_t)d1 * 2;       /* double count the square divisor */
            } else {
                sum += (seq_t)d1 + (seq_t)d2;
            }
        }
    }
    return sum;
}
