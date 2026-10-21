/*
 * A217973  Niven (or Harshad) numbers not containing the digit 0.
 * https://oeis.org/A217973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth Niven (Harshad) number that contains no digit 0
 * Range:     u8 n <= 49, u16 n <= 4149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, has_no_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/has_no_zero_digit.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1; n must be >=1 */
    idx_t count = 0;
    seq_t cand = 1;

    while (count < n) {
        if (has_no_zero_digit(cand)) {
            seq_t sum = digit_sum(cand);
            if (sum != 0 && cand % sum == 0) {
                ++count;
                if (count == n) return cand;
            }
        }
        ++cand;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
