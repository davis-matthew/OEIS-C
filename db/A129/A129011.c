/*
 * A129011  a(n) = floor(n^(4/3)).
 * https://oeis.org/A129011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n^(4/3))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   cube_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes n^4 exactly in seq_t
 * and then finds the integer cube root by binary search.
 * It uses only integer operations allowed in the specification. */

#include <stdint.h> /* for NULL, types, if needed */
#include "../utils/cube_leq.h"

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return 0;

    /* Compute n^2 and n^4 in seq_t.  Safe for all n that occur in tests
     * because n^4 fits in seq_t for the supplied build parameters. */
    seq_t nn = (seq_t)n * n;               /* n^2 */
    seq_t target = nn * nn;               /* n^4 */

    /* Binary search for floor(cuberoot(n^4)) in [0, n^2] */
    seq_t low  = 0;
    seq_t high = nn;  /* n^2 is an upper bound for the cube root */

    while (low <= high) {
        seq_t mid = (low + high) >> 1;    /* mid = floor((low+high)/2) */
        if (cube_leq(mid, target)) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return high;  /* floor(n^(4/3)) */
}
