/*
 * A152833  a(0) = -3; a(n) = n-a(n-1).
 * https://oeis.org/A152833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n + 1 - 13*(-1)^n)/4 (OEIS A152833)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use the closed‐form expression derived from the recurrence
       a(0) = -3; a(n) = n - a(n-1).  The value can be computed in
       constant time using integer arithmetic. */
    if ((n & 1) == 0) {               /* n even:  a(n) = (n - 6)/2 */
        return ((seq_t)n - 6) / 2;
    } else {                           /* n odd:   a(n) = (n + 7)/2 */
        return ((seq_t)n + 7) / 2;
    }
}
