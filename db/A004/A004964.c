/*
 * A004964  a(n) = ceiling(n*phi^9), where phi is the golden ratio, A001622.
 * https://oeis.org/A004964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n * phi^9), where phi = (1+sqrt(5))/2 and phi^9 =
 *            38 + 17*sqrt(5)
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    uint64_t y = x;
    uint64_t z = (x + 1) / 2;
    while (z < y) {
        y = z;
        z = (y + x / y) / 2;
    }
    return y;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t n_seq = (seq_t)n;
    seq_t part1 = 38 * n_seq;
    
    /* Compute floor(17*n*sqrt(5)) = floor(sqrt(1445*n^2)) */
    uint64_t n64 = (uint64_t)n;
    uint64_t val = 1445ULL * n64 * n64;
    uint64_t floor_17nsqrt5 = isqrt_u64(val);
    
    seq_t part2 = (seq_t)floor_17nsqrt5;
    
    return part1 + part2 + 1;
}
