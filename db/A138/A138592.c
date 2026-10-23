/*
 * A138592  Sums of 4 or more consecutive nonnegative integers.
 * https://oeis.org/A138592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_sum_of_consecutive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138592: sums of 4 or more consecutive nonnegative integers.
   The function enumerates all such sums and returns the n-th smallest
   distinct value using only integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_sum_of_consecutive.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   // undefined for offset 1, but safe return
    idx_t count = 0;
    seq_t num = 0;
    for (num = 0; ; ++num) {
        if (num < 6) continue;   // first representable sum is 6
        if (is_sum_of_consecutive(num)) {
            ++count;
            if (count == n) return num;
        }
    }
}
