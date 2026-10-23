/*
 * A019529  Sum of a(n) terms of 1/sqrt(k) first strictly exceeds n.
 * https://oeis.org/A019529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest k such that b(k) > n, where
   b(k) = 1 + 1/√2 + 1/√3 + … + 1/√k. */
/* The function merely computes the sum incrementally until it
   exceeds the integer target n. */

#include <math.h>

seq_t a(idx_t n) {
    double target = (double)n;          /* target integer as double */
    double sum    = 0.0;                /* cumulative sum of 1/√k */

    for (idx_t k = 1; ; ++k) {
        sum += 1.0 / sqrt((double)k);
        if (sum > target) return (seq_t)k;
    }
}
