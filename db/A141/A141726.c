/*
 * A141726  Sawtooth with period length 9: repeat 8, 7, 6, 5, 4, 3, 2, 1, 0.
 * https://oeis.org/A141726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 - ((n-1) mod 9)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* a(1) = 8, a(2) = 7, ..., a(9) = 0, repeat with period 9 */
    idx_t rem = (n - 1) % 9;
    return (seq_t)(8 - rem);
}
