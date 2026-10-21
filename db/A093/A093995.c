/*
 * A093995  n^2 appears n times, triangle read by rows.
 * https://oeis.org/A093995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(sqrt(2*n - 1) + 1/2)^2 (OEIS A093995)
 * Range:     u8 n <= 32, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 */
    seq_t S = (seq_t)n * 2 - 1;          /* S = 2*n - 1 */
    seq_t s = floor_sqrt(S);             /* s = floor(sqrt(S)) */
    seq_t t = s * (s + 1) + 1;           /* threshold for rounding up */
    seq_t m = (S >= t) ? (s + 1) : s;    /* nearest integer to sqrt(S) */
    return m * m;                        /* square of the nearest integer */
}
