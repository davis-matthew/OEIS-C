/*
 * A051624  12-gonal (or dodecagonal) numbers: a(n) = n*(5*n-4).
 * https://oeis.org/A051624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n-4)
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)          /* avoid underflow of unsigned arithmetic */
        return 0;
    seq_t tmp = (seq_t)n * (seq_t)5 - (seq_t)4;
    return (seq_t)n * tmp;
}
