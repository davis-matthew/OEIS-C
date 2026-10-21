/*
 * A219652  Number of steps to reach 0 starting with n and using the iterated
 *          process: x -> x - (sum of digits in factorial expansion of x).
 * https://oeis.org/A219652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n > 0, a(n) = 1 + a( n -
 *            sum_of_digits_in_factorial_base(n) ) where the digit sum is
 *            taken in the factorial number system
 * Range:     u8 n <= 1813, u16 n <= 9778, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   factorial_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factorial_digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t steps = 0;
    idx_t cur   = n;
    while (cur > 0) {
        seq_t sub = factorial_digit_sum(cur);
        cur = (idx_t)(cur - sub);
        ++steps;
    }
    return steps;
}
