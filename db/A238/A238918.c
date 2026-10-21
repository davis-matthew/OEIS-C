/*
 * A238918  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 7.
 * https://oeis.org/A238918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of self-inverse permutations on [n] whose maximal
 *            displacement equals 7. It is computed as the difference between
 *            the number of involutions with all displacements ≤ 7 and those
 *            with all displacements ≤ 6
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   count_leq_displacement
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic programming that keeps, for each processed position,
   a bitmask describing the pending transpositions that are still unmatched.
   The bitmasks represent distances (1…maxD) from the current position to the other
   element of the pending transposition.  The DP ensures that no pending element is ever
   allowed to exceed the maximum allowed displacement. */

/* Dependencies:
   The code relies on the types seq_t and idx_t defined in the build environment
   and on standard integer operators.  No external libraries are required. */
#include <stdbool.h>
#include "../utils/count_leq_displacement.h"

seq_t a(idx_t n)
{
    /* For n < 8 the maximal possible displacement is < 7, so the answer is 0. */
    if (n < 8) return 0;

    seq_t f7 = count_leq_displacement(n, 7);
    seq_t f6 = count_leq_displacement(n, 6);
    return f7 - f6;
}
