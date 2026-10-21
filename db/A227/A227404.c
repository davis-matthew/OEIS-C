/*
 * A227404  Total number of inversions in all permutations of order n
 *          consisting of a single cycle.
 * https://oeis.org/A227404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (3*n-1) / 12 for n > 2 a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1) return 0;
    if (n == 2) return 1;

    /* Compute n! while cancelling the factor 12 as we go to avoid overflow.
       After the loop, divisor will hold any remaining part of 12 that
       could not be cancelled yet, which will be 1 for n >= 3. */
    seq_t fact = 1;
    seq_t divisor = 12;

    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
        /* Cancel any common factor with the remaining divisor */
        seq_t g = gcd(fact, divisor);
        if (g > 1) {
            fact /= g;
            divisor /= g;
        }
    }

    /* Multiply by (3*n-1) and finally cancel any leftover divisor
       (this will be 1 for n >= 3). */
    seq_t term = (seq_t)(3 * n - 1);
    fact *= term;

    if (divisor > 1) fact /= divisor;

    return fact;
}
