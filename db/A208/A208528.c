/*
 * A208528  Number of permutations of n>1 having exactly 3 points P on the
 *          boundary of their bounding square.
 * https://oeis.org/A208528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-8) * (n-2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the definition is not used; return 0 safely. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute (n-2)! iteratively. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 2; ++i) {
        fact *= (seq_t)i;
    }

    /* Multiply by (4*n-8).  Use seq_t conversion to keep types consistent. */
    seq_t mult = ((seq_t)4 * (seq_t)n) - (seq_t)8;
    return fact * mult;
}
