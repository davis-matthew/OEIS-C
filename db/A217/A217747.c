/*
 * A217747  Numbers whose digits sum to a perfect number.
 * https://oeis.org/A217747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose decimal digit sum is a
 *            perfect number
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

static const seq_t perfects[] = {
    (seq_t)6,
    (seq_t)28
};
static const size_t perfects_cnt = sizeof(perfects) / sizeof(perfects[0]);

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t count = 0;
    seq_t val = (seq_t)0;

    while (count < n) {
        val++;                        /* next integer to test */
        seq_t s = digit_sum(val);     /* sum of decimal digits */

        for (size_t i = 0; i < perfects_cnt; ++i) {
            if (s == perfects[i]) {
                ++count;
                break;
            }
        }
    }
    return val;
}
