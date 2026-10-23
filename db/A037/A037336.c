/*
 * A037336  Numbers whose base-9 and base-10 expansions have the same digit
 *          sum.
 * https://oeis.org/A037336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum_base10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037336: numbers whose base-9 and base-10 digit sums are equal */
/* Computed by enumerating integers and checking digit sum equality between base-9 and base-10 */
#include <stdint.h> /* idx_t and seq_t are defined by the build environment */
#include "../utils/digit_sum_base10.h"

static inline idx_t digit_sum_base9(idx_t x) {
    idx_t sum = 0;
    while (x) {
        sum += x % 9;
        x /= 9;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, return 0 as placeholder */

    idx_t count = 0;
    for (idx_t curr = 1; ; ++curr) {
        if (digit_sum_base10(curr) == digit_sum_base9(curr)) {
            ++count;
            if (count == n) {
                return (seq_t)curr;
            }
        }
    }
}
