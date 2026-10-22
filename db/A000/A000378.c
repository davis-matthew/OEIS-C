/*
 * A000378  Sums of three squares: numbers of the form x^2 + y^2 + z^2.
 * https://oeis.org/A000378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_sum_of_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate through nonnegative integers and count those that are sums of three squares */

#include <stdbool.h>
#include "../utils/is_sum_of_three_squares.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;
    while (1) {
        if (is_sum_of_three_squares(num)) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num++;
    }
}
