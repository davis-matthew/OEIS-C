/*
 * A133292  Period 9: repeat [1, 1, 2, 4, 7, 2, 7, 4, 2].
 * https://oeis.org/A133292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = [1, 1, 2, 4, 7, 2, 7, 4, 2][n mod 9]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t period[9] = {1, 1, 2, 4, 7, 2, 7, 4, 2};
    return period[(size_t)(n % 9)];
}
