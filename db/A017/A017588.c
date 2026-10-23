/*
 * A017588  a(n) = (12*n + 5)^8.
 * https://oeis.org/A017588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 5)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute base = 12*n + 5 */
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)5;

    /* Raise base to the 8th power using repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i) {
        result *= base;
    }
    return result;
}
