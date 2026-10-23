/*
 * A177885  a(n) = (1-n)^(n-1).
 * https://oeis.org/A177885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1-n)^(n-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = (1-n)^(n-1).  Uses exponentiation by squaring.
   For n == 0, returns 1 as defined (0th term of the sequence). */
extern seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t base = (seq_t)1 - (seq_t)n;   /* base = 1 - n */
    idx_t    exp  = n - 1;              /* exponent = n-1 */
    seq_t    result = (seq_t)1;

    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}
