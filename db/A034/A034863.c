/*
 * A034863  a(n) = n!*(4*n^3 - 30*n^2 + 40*n + 3)/24.
 * https://oeis.org/A034863
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*(4*n^3 - 30*n^2 + 40*n + 3)/24 (OEIS A034863)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"


seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4; for lower indices the value is unspecified. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Compute the polynomial part: 4*n^3 - 30*n^2 + 40*n + 3 */
    seq_t nn   = (seq_t)n;
    seq_t poly = (seq_t)4 * nn * nn * nn
               - (seq_t)30 * nn * nn
               + (seq_t)40 * nn
               + (seq_t)3;

    /* factorial(n) is guaranteed to be divisible by 24 for n >= 4. */
    seq_t fact = factorial(n);   /* factorial from utils, returns seq_t */
    fact = fact / (seq_t)24;     /* integer division, exact */

    return fact * poly;
}
