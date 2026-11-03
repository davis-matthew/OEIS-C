/*
 * A103848  Numbers k such that sum of even digits of k is less than sum of
 *          odd digits.
 * https://oeis.org/A103848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8923 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer k for which the sum of its even decimal digits
   is less than the sum of its odd decimal digits. */
#include <stdint.h>

static int qualifies(uint64_t x)
{
    uint64_t tmp = x;
    uint64_t sum_even = 0, sum_odd = 0;
    while (tmp > 0) {
        uint64_t d = tmp % 10;
        if ((d & 1) == 0)
            sum_even += d;
        else
            sum_odd += d;
        tmp /= 10;
    }
    return sum_even < sum_odd;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not used, offset is 1 */

    uint64_t k = 1;
    uint64_t count = 0;

    while (count < n) {
        if (qualifies(k)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }

    return (seq_t)0;   /* unreachable */
}
