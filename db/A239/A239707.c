/*
 * A239707  Number of bases b for which the base-b alternate digital sum of n
 *          is 0.
 * https://oeis.org/A239707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{b ∈ [1, n−1] : altDigitSum_b(n) = 0} a(1) = 0 (for n ≥
 *            2: base 1 gives 0 iff n is even, and base b = n−1 always works).
 *            n is a 64‑bit unsigned index; computing alt‑digital sums is
 *            O(n·log_b n), fast enough for n up to about 10 000
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system (see the specification). */

seq_t a(idx_t n)
{
    if (n == 1)                     /* a(1) = 0 */
        return 0;

    seq_t count = 0;

    /* Base 1: altDigitSum_1(n) = n mod 2. */
    if ((n & 1ULL) == 0)
        count++;

    /* Count admissible bases from 2 up to n−2. */
    for (idx_t b = 2; b <= n - 2; ++b) {
        idx_t tmp = n;
        uint64_t sum_odd  = 0;   /* digits in positions 0,2,4...  */
        uint64_t sum_even = 0;   /* digits in positions 1,3,5...  */
        bool odd_pos = true;     /* least significant digit is position 0 */

        while (tmp > 0) {
            uint64_t digit = tmp % b;
            if (odd_pos)
                sum_odd += digit;
            else
                sum_even += digit;
            tmp /= b;
            odd_pos = !odd_pos;
        }

        /* The base works iff sum_odd == sum_even.  The totals fit in uint64_t
           because sum_odd + sum_even ≤ n·2 and n ≤ 2^64−1. */
        if (sum_odd == sum_even)
            count++;
    }

    /* Base b = n−1 always yields altDigitSum_b(n) = 0.
       For n = 2 this base coincides with base 1 already counted.   */
    if (n > 2)
        count++;

    return count;
}
