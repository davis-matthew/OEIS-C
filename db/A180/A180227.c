/*
 * A180227  Triangular array T(A,B) read by rows: minimal number of steps
 *          required to obtain exactly 1 liter in jug A (irrespective of jug
 *          B), starting with infinite supply of water and two empty jugs with
 *          capacities A and B liters. -1 if not possible. A>=B>=1.
 * https://oeis.org/A180227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 2274, i16 n <= 8169, i32 n <= 8169, i64 n <= 8169,
 *            i128 n <= 8169, big any n
 * Helpers:   gcd, steps_a_to_b, steps_b_to_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: minimal number of steps to get exactly 1 liter in jug A
   (or -1 if impossible), as defined by OEIS sequence A180227. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/steps_a_to_b.h"
#include "../utils/steps_b_to_a.h"

seq_t a(idx_t n) {
    /* Map index n to (A,B).  A >= B >= 1. */
    idx_t low = 1, high = 1;
    /* increase high until triangular(high) >= n */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t A = low;
    idx_t prev_tri = ((A - 1) * A) / 2;
    idx_t B = n - prev_tri;

    /* Check solvability */
    if (gcd((seq_t)A, (seq_t)B) != 1)
        return (seq_t)-1;

    /* Compute minimal steps */
    seq_t step1 = steps_a_to_b(A, B, 1);
    seq_t step2 = steps_b_to_a(A, B, 1);
    return (step1 < step2) ? step1 : step2;
}
