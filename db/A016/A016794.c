/*
 * A016794  a(n) = (3*n + 2)^6.
 * https://oeis.org/A016794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute w = 3*n + 2 */
    seq_t w = (seq_t)n;   /* start from the index as seq_t */
    w = w * 3 + 2;

    /* Compute w^6 in a chain of multiplications to keep intermediates small */
    seq_t w2 = w * w;      /* w^2 */
    seq_t w4 = w2 * w2;    /* w^4 */
    seq_t w5 = w4 * w;     /* w^5 */
    seq_t w6 = w5 * w;     /* w^6 */

    return w6;
}
