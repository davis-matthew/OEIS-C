/*
 * A093435  Duplicate of A048964.
 * https://oeis.org/A093435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th positive integer that is not in the set {1,4,9,13}
 * nor congruent to 2 modulo 4 for all numbers ≥18.
 * The function searches sequentially for the n-th element. */
#include <stdint.h>
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1;; ++m) {
        /* Skip the special excluded numbers */
        if (m == 1 || m == 4 || m == 9 || m == 13 ||
            (m >= 18 && (m & 3) == 2)) {
            continue;
        }
        ++count;
        if (count == n)
            return (seq_t)m;
    }
}
