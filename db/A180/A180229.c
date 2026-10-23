/*
 * A180229  Triangular array T(A,B) read by rows: minimal number of steps
 *          required to obtain exactly 3 liters in jug A (irrespective of jug
 *          B), starting with infinite supply of water and two empty jugs with
 *          capacities A and B liters. -1 if not possible. A>=B>=1.
 * https://oeis.org/A180229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 1701, i16 n <= 9912, i32 n <= 9912, i64 n <= 9912,
 *            i128 n <= 9912, big any n
 * Helpers:   gcd, steps_strategy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180229: minimal number of steps to obtain exactly 3 liters in jug A
   for capacities A>=B>=1.  The function simulates the two standard
   jug algorithms (pouring from A→B or B→A) and returns the smallest
   number of steps, or -1 if it is impossible. */

#include "../utils/gcd.h"
#include "../utils/steps_strategy.h"

seq_t a(idx_t n)
{
    const seq_t target = (seq_t)3;

    /* Compute row A and column B from the triangular index n */
    idx_t A = 1;
    while ((A * (A + 1)) / 2 < n) A++;
    idx_t offset = ((A - 1) * A) / 2;
    idx_t B = n - offset; /* 1 <= B <= A */

    /* Quick impossible tests */
    if (target > (seq_t)A) return -1;                 /* target larger than A */
    seq_t g = gcd((seq_t)A, (seq_t)B);
    if (target % g != 0) return -1;                  /* not a multiple of gcd */

    seq_t steps1 = steps_strategy(A, B, 0, target);   /* pour A→B */
    seq_t steps2 = steps_strategy(A, B, 1, target);   /* pour B→A */

    return (steps1 < steps2) ? steps1 : steps2;
}
