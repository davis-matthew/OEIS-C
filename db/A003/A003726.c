/*
 * A003726  Numbers with no 3 adjacent 1's in binary expansion.
 * https://oeis.org/A003726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th number with no three consecutive 1's in binary;
 *            computed by iteration
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_three_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_three_ones.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (1) {
        if (!has_three_ones(num)) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num++;
    }
}
