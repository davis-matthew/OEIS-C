/*
 * A130251  Partial sums of A130249.
 * https://oeis.org/A130251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} floor(log_2(3*k+1))
 * Range:     u8 n <= 47, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(v)) for v > 0 using the 64‑bit builtin */
static inline unsigned int floor_log2_u64(uint64_t v) {
    return 63U - __builtin_clzll(v);
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t val = (uint64_t)3 * k + 1;   /* 3*k+1 > 0 for k >= 1 */
        unsigned int l = floor_log2_u64(val);
        sum += (seq_t)l;
    }
    return sum;
}
