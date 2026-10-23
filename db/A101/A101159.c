/*
 * A101159  Let j be the smallest integer for which n+(n+1)+...+(n+j) is a
 *          square; then a(n) = n+j.
 * https://oeis.org/A101159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 369, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For each n find the minimal integer m ≥ n such that the sum
   n + (n+1) + … + m is a perfect square.  The function returns that m
   (the OEIS indexing convention a(n) = m).  The search stops when
   m ≤ 3·n − 2, which is guaranteed by the inequality a(n) ≤ 3 n − 2. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Handle offset: sequence is defined for n ≥ 1. */
    if (n == 0) return 0;

    /* The upper bound from the OEIS inequality: a(n) ≤ 3 n − 2. */
    idx_t max_m = 3 * n - 2;
    if (max_m < n)        /* happens only for n == 1 */
        max_m = n;

    seq_t sum = 0;          /* current running sum n + … + m */
    for (idx_t m = n; m <= max_m; ++m) {
        sum += (seq_t)m;    /* add the new term m */

        /* Check if sum is a perfect square. */
        seq_t r = isqrt(sum);
        if (r * r == sum) {
            return (seq_t)m;
        }
    }

    /* Theoretical guarantee: a(n) never exceeds 3·n − 2, so we should
       never reach here.  Return the last checked m as a fallback. */
    return (seq_t)max_m;
}
