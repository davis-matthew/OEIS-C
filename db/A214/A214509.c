/*
 * A214509  a(n) = 1 if n is an odd square or twice a nonzero even square, -1
 *          if a nonzero even square or twice an odd square else 0.
 * https://oeis.org/A214509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 1 if n is an odd square or twice a nonzero even square, *
 *            -1 if n is a nonzero even square or twice an odd square, * 0
 *            otherwise. (OEIS A214509)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Indexes must be >= 1 in the OEIS sequence.  Return 0 for 0 just
       to guard against accidental calls. */
    if (n == 0)
        return 0;

    /* Test whether n itself is a perfect square. */
    idx_t s = _floor_isqrt_u64(n);
    if (s * s == n) {
        /* n is a square.  Odd square → 1, even square → -1. */
        return (s & 1) ? (seq_t)1 : (seq_t)-1;
    }

    /* If n is even, test whether n/2 is a perfect square. */
    if ((n & 1u) == 0) {
        idx_t m = n >> 1;          /* m = n/2 */
        idx_t t = _floor_isqrt_u64(m);
        if (t * t == m) {
            /* t even (even square) → 1, t odd (odd square) → -1. */
            return (t & 1) ? (seq_t)-1 : (seq_t)1;
        }
    }

    /* All other cases yield 0. */
    return 0;
}
