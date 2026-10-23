/*
 * A135677  a(n) = ceiling(n^(4/3)+n).
 * https://oeis.org/A135677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(4/3)+n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 288, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for n < 1 */
    seq_t n_s = (seq_t)n;

    /* Check if n is a perfect cube */
    idx_t c = int_cube_root(n);                /* floor(cuberoot(n)) */
    seq_t c3 = (seq_t)c * (seq_t)c * (seq_t)c;
    bool is_cube = (c3 == n_s);

    /* Compute n^4 as a seq_t to avoid overflow */
    seq_t n_sq = n_s * n_s;
    seq_t n4   = n_sq * n_sq;

    /* Upper bound for floor(n^(4/3)) */
    seq_t high = n_s * (seq_t)(c + 1);
    seq_t low  = 0;

    /* Binary search for the largest k such that k^3 <= n^4 */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= n4)
            low = mid;
        else
            high = mid - 1;
    }
    seq_t floor_n4_3 = low;
    seq_t ceil_n4_3  = floor_n4_3 + (is_cube ? 0 : 1);

    return n_s + ceil_n4_3;
}
