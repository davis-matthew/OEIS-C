/*
 * A028283  Central elements in 4-Pascal triangle A028275 (by row).
 * https://oeis.org/A028283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A000108(n-1)*(3*n-1), with a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided catalan(idx_t) helper from utils/catalan.h */
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t cat = catalan(n - 1);
    seq_t mul = (seq_t)(3 * n - 1);
    return 2 * cat * mul;
}
