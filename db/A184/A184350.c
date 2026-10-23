/*
 * A184350  Number of strings of numbers x(i=1..4) in 0..n with sum i^4*x(i)
 *          equal to 256*n.
 * https://oeis.org/A184350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integer solutions (x1,x2,x3,x4) with 0 <= xi <= n
 *            and x1 + 16*x2 + 81*x3 + 256*x4 = 256*n for n >= 1. Computed by
 *            iterating over x4 and x3 and counting admissible x2 values in
 *            O(n^2/81) operations
 * Range:     u8 n <= 39, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t x4 = 0; x4 <= n; ++x4) {
        /* Remaining sum after fixing x4 */
        idx_t L = (idx_t)256 * (n - x4);
        /* Max possible x3 given L and bound n */
        idx_t max_x3 = L / 81;
        if (max_x3 > n) max_x3 = n;
        for (idx_t x3 = 0; x3 <= max_x3; ++x3) {
            idx_t R = L - 81 * x3; /* Remaining after x3 */
            /* Compute admissible range for x2:
               0 <= R - 16*x2 <= n  <=>  (R-n)/16 <= x2 <= R/16  */
            idx_t low16; /* ceil((R-n)/16) or 0 if R <= n */
            if (R > n) {
                low16 = (R - n + 15) / 16;
            } else {
                low16 = 0;
            }
            idx_t high16 = R / 16; /* floor(R/16) */
            if (low16 > n) continue; /* no x2 in bound */
            if (high16 > n) high16 = n;
            if (high16 >= low16) {
                total += (seq_t)(high16 - low16 + 1);
            }
        }
    }
    return total;
}
