/*
 * A067085  a(n) = floor(Sum_{k=1..n} 1/k^(1/2)).
 * https://oeis.org/A067085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(∑_{k=1..n} 1/√k)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_64.h"

seq_t a(idx_t n) {
    const unsigned long long SCALE = 1000000ULL;   /* 1e6 scaling factor */
    unsigned long long sum_scaled = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long prod = k * SCALE * SCALE;        /* k * 1e12  < 2^64 */
        unsigned long long sqrt_scaled = isqrt_64(prod);    /* floor(√(k)*SCALE) */
        unsigned long long term = (SCALE * SCALE) / sqrt_scaled; /* floor(SCALE/√k) * SCALE? */
        sum_scaled += term;
    }
    seq_t result = (seq_t)(sum_scaled / SCALE);
    return result;
}
