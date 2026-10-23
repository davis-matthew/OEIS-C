/*
 * A049393  Expansion of (1-25*x)^(1/5).
 * https://oeis.org/A049393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 15, i128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049393
   Formula (product form):
          a(n) = 5^n / n! * ∏_{k=0}^{n-1} (5*k - 1)
   Recurrence equivalent:
          a(0) = 1
          a(n) = a(n-1) * (25*n - 30) / n   for n ≥ 1
*/

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* numerator part of the recurrence */
        seq_t num = (seq_t)25 * (seq_t)i - (seq_t)30;

        /* Reduce to avoid overflow before multiplication */
        seq_t g1 = gcd(result, (seq_t)i);
        seq_t a1 = result / g1;
        seq_t c1 = (seq_t)i / g1;

        seq_t g2 = gcd(num, c1);
        seq_t b1 = num / g2;
        seq_t c2 = c1 / g2;

        /* Compute the next term */
        result = (a1 * b1) / c2;
    }
    return result;
}
