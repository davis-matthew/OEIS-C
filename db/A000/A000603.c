/*
 * A000603  Number of nonnegative solutions to x^2 + y^2 <= n^2.
 * https://oeis.org/A000603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative solutions to x^2 + y^2 <= n^2
 * Range:     u8 n <= 15, u16 n <= 259, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n_squared = nn * nn;
    seq_t count = 0;
    
    for (seq_t x = 0; x <= nn; x++) {
        seq_t x_squared = x * x;
        seq_t remaining = n_squared - x_squared;
        seq_t max_y = isqrt(remaining);
        count += max_y + 1;
    }
    
    return count;
}
