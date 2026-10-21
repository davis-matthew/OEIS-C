/*
 * A055274  First differences of 8^n (A001018).
 * https://oeis.org/A055274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 7 * 8^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset explicitly */
    if (n == 0) return (seq_t)1;

    /* Compute 8^(n-1) */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < (n - 1); ++i) {
        power *= (seq_t)8;
    }

    /* Compute the term */
    return (seq_t)7 * power;
}
