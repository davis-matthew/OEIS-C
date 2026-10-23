/*
 * A017638  a(n) = (12*n + 9)^10.
 * https://oeis.org/A017638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the base value (12*n + 9).  Multiplication is performed
       in seq_t to avoid intermediate overflow of idx_t. */
    seq_t base = (seq_t)n * 12 + 9;

    /* Compute the 10th power by repeated multiplication.
       Uses only +, -, *, /, % and assignment, which are allowed on seq_t. */
    seq_t result = base;
    for (int i = 1; i < 10; ++i) {
        result *= base;
    }
    return result;
}
