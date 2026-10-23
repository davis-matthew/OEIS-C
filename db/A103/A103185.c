/*
 * A103185  a(n) = Sum_{ k >= 0 such that n + k == 0 mod 2^k } 2^(k-1).
 * https://oeis.org/A103185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{k>=1, n+k == 0 mod 2^k} 2^(k-1)
 * Range:     u8 n <= 502, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= 63; ++k) {
        uint64_t mask = ((uint64_t)1 << k) - 1;
        uint64_t lower = ((uint64_t)n) & mask;
        if (((lower + k) & mask) == 0) {
            unsigned long long power = 1ULL << (k - 1);
            sum += (seq_t)power;
        }
    }
    return sum;
}
