/*
 * A024054  a(n) = 5^n - n^5.
 * https://oeis.org/A024054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^5
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* For uint64_t definition, if needed */

seq_t a(idx_t n) {
    /* Compute 5^n */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
    }

    /* Compute n^5 via successive multiplication */
    seq_t n5 = (seq_t)n;
    n5 *= n5;          /* n^2 */
    n5 *= (seq_t)n;    /* n^3 */
    n5 *= (seq_t)n;    /* n^4 */
    n5 *= (seq_t)n;    /* n^5 */

    return pow5 - n5;
}
