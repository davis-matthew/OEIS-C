/*
 * A006021  The game of contours.
 * https://oeis.org/A006021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+8) = a(n) for n >= 6, with a(0..13) =
 *            0,1,2,3,1,4,3,2,0,5,2,3,1,4
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const unsigned char init_terms[14] = {
    0, 1, 2, 3, 1, 4, 3, 2, 0, 5, 2, 3, 1, 4
};

seq_t a(idx_t n) {
    if (n < 14) return (seq_t)init_terms[n];
    return (seq_t)init_terms[6 + (n - 6) % 8];
}
