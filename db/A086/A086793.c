/*
 * A086793  Number of iterations of the map A034690 (x -> sum of digits of all
 *          divisors of x) required to reach one of the fixed points, 15 or 1.
 * https://oeis.org/A086793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of iterations of the map x → sum of digit sums of
 *            all divisors of x required to reach the fixed points 15 or 1
 *            (a(1)=0, a(15)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_unsigned, sum_divisor_digit_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum_unsigned.h"
#include "../utils/sum_divisor_digit_sums.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;
    seq_t steps = 0;
    idx_t cur = n;
    while (cur != 1 && cur != 15) {
        cur = (idx_t)sum_divisor_digit_sums(cur);
        ++steps;
    }
    return steps;
}
