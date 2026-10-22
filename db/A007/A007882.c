/*
 * A007882  Number of lattice points inside circle of radius n is
 *          4*(a(n)+n)-3.
 * https://oeis.org/A007882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

/* Count of ordered pairs (x, y) with x, y > 0 and x^2 + y^2 < n^2 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    
    for (seq_t x = 1; x * x < n_sq; x++) {
        seq_t remaining = n_sq - x * x;
        uint64_t y_max = a000037_isqrt((uint64_t)remaining - 1);
        count += (seq_t)y_max;
    }
    
    return count;
}
