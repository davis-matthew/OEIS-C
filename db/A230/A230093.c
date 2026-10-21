/*
 * A230093  Number of values of k such that k + (sum of digits of k) is n.
 * https://oeis.org/A230093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k | 0≤k≤n, k + digit_sum(k) = n }
 * Range:     u8 n <= 1006, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over possible digit sums d (0≤d≤9⋅digits(n)),
   set k = n - d, and check whether digit_sum(k) == d. */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* number of decimal digits of n (1 for n = 0) */
    idx_t ndigits = digit_count((uint64_t)n);
    seq_t count = 0;

    for (idx_t d = 0; d <= 9 * ndigits && d <= n; ++d) {
        idx_t k = n - d;
        if (digit_sum((seq_t)k) == (seq_t)d)
            ++count;
    }

    return count;
}
