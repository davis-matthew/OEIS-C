/*
 * A104159  Numbers on a Manchester or Log-End dartboard, as read in a
 *          standard, clockwise direction.
 * https://oeis.org/A104159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[(n-1) % 20] where pattern =
 *            {4,20,1,16,6,17,8,12,9,14,5,19,2,15,3,18,7,11,10,13}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    static const seq_t pattern[20] = {
        4,20,1,16,6,17,8,12,9,14,5,19,2,15,3,18,7,11,10,13
    };
    if (n == 0) return 0;          /* for n<1 the value is undefined */
    idx_t idx = (n - 1) % 20;
    return pattern[(size_t)idx];
}
