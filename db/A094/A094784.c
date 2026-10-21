/*
 * A094784  Numbers that are neither squares nor cubes.
 * https://oeis.org/A094784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A094784: Numbers that are neither squares nor cubes.
   The function searches for the nth such integer by testing each candidate
   and skipping perfect squares and cubes.  It uses the utility functions
   floor_sqrt and floor_cube_root which return the integer floor of the
   square/cube root.  The algorithm is O(a(n)), which is fast enough for
   n up to about 10000, as required. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

static inline bool is_square(seq_t x) {
    seq_t s = floor_sqrt(x);
    return (s * s == x);
}

static inline bool is_cube(seq_t x) {
    seq_t r = floor_cube_root(x);
    return (r * r * r == x);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for offset 0, return 0 */
    idx_t count = 0;
    seq_t val = 2;                 /* Candidates start at 2 */
    for (;;) {
        if (!is_square(val) && !is_cube(val)) {
            ++count;
            if (count == n) return val;
        }
        ++val;
    }
}
