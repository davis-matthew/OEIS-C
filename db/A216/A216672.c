/*
 * A216672  Total number of solutions to the equation x^2 + k*y^2 = n with x >
 *          0, y > 0, k > 0. (Order does not matter for the equation x^2 + y^2
 *          = n.)
 * https://oeis.org/A216672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A216672
   a(n) = Total number of positive integer solutions (x,y,k) of
   x^2 + k*y^2 = n with k>0, x>0, y>0.
   For k = 1 the unordered pair (x,y) and (y,x) is counted only once.
   The function iterates over all k (1 ≤ k < n) and over
   1 ≤ y ≤ ⌊√(n/k)⌋, checking whether the remaining value is a perfect
   square.  For k = 1 we only count solutions with y ≤ x to avoid
   double counting.  Complexity is O(n). */
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    seq_t total = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (k > n) break;                  // safety; k < n anyway
        idx_t max_y = _floor_isqrt_u64(n / k);
        for (idx_t y = 1; y <= max_y; ++y) {
            idx_t rem = n - k * y * y;
            if (rem == 0) continue;        // requires x > 0
            idx_t x = _floor_isqrt_u64(rem);
            if ((seq_t)x * (seq_t)x == rem) {
                if (k == 1 && y > x) continue; // unordered for k=1
                ++total;
            }
        }
    }
    return total;
}
