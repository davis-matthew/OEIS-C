/*
 * A001182  Number of cells of square lattice of edge 1/n inside quadrant of
 *          unit circle centered at 0.
 * https://oeis.org/A001182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} floor(sqrt(n^2-k^2))
 * Range:     u8 n <= 16, u16 n <= 280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    
    seq_t result = 0;
    seq_t nn = (seq_t)n * (seq_t)n;
    
    for (idx_t k = 1; k < n; k++) {
        seq_t kk = (seq_t)k * (seq_t)k;
        seq_t diff = nn - kk;
        seq_t sqrtval = isqrt_seq(diff);
        result += sqrtval;
    }
    
    return result;
}
