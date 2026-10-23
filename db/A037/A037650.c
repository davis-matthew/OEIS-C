/*
 * A037650  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; for n>1, a(n) = a(n-1) * 8 + d(n-1), where the digit
 *            sequence d repeats 3,0,1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t DIGITS[3] = {3, 0, 1};

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)8;
        res += DIGITS[i % 3];
    }
    return res;
}
