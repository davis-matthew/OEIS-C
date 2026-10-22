/*
 * A001745  Numbers such that at least one digit contains a loop (version 2).
 *          Also called "holey" or "holy" numbers.
 * https://oeis.org/A001745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th number with at least one holey digit (0, 4, 6,
 *            8, 9)
 * Range:     u8 n <= 182, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_holey_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/has_holey_digit.h"

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t num = 0;
    while (1) {
        if (has_holey_digit(num)) {
            count++;
            if (count == target) {
                return num;
            }
        }
        num++;
    }
}
