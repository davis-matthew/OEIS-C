/*
 * A134029  Period 9: repeat 3, 2, 4, 1, 5, 1, 4, 2, 3.
 * https://oeis.org/A134029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [3,2,4,1,5,1,4,2,3][n mod 9]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t period[9] = {3, 2, 4, 1, 5, 1, 4, 2, 3};

seq_t a(idx_t n) {
    return period[n % 9];
}
