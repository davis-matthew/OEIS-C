/*
 * A017171  a(n) = (9*n)^11.
 * https://oeis.org/A017171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 353,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute base = 9*n */
    seq_t base = (seq_t)9 * (seq_t)n;
    seq_t result = 1;
    for (int i = 0; i < 11; ++i) {
        result *= base;
    }
    return result;
}
