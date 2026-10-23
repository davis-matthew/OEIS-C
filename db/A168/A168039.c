/*
 * A168039  Squares closest to 3*n.
 * https://oeis.org/A168039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest perfect square to 3*n
 * Range:     u8 n <= 10, u16 n <= 976, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) computed by binary search */
static inline seq_t isqrt_seq(seq_t x) {
    seq_t low = 0;
    seq_t high = x + 1;          /* upper bound */
    while (high - low > 1) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n * 3;                    /* 3*n */
    seq_t k = isqrt_seq(target);                   /* floor(sqrt(3*n)) */
    seq_t k2 = k * k;
    seq_t k1 = k + 1;
    seq_t k1sq = k1 * k1;
    seq_t diff1 = target - k2;                     /* target - k^2   */
    seq_t diff2 = k1sq - target;                   /* (k+1)^2 - target */
    return (diff1 <= diff2) ? k2 : k1sq;            /* nearest square */
}
