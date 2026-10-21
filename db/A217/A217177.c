/*
 * A217177  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains two consecutive identical
 *          digits.
 * https://oeis.org/A217177
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_len, no_consecutive_equal_digits, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of digits of the smallest power of n that contains two consecutive identical digits. */
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/digit_len.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* offset starts at 2 */

    idx_t k = 1;
    while (1) {
        seq_t cur = pow_uint((uint64_t)n, k);
        if (!no_consecutive_equal_digits(cur)) {
            return (seq_t)digit_len(cur);
        }
        ++k;
    }
}
