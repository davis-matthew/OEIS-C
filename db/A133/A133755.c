/*
 * A133755  Number of positive integers less than n/3 that are coprime to n.
 * https://oeis.org/A133755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k with 1 <= k < n/3 and gcd(k, n) = 1
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n <= 3 the range n/3 gives zero positive integers. */
    if (n <= 3)
        return 0;

    seq_t count = 0;
    seq_t seq_n = (seq_t)n;

    /* The largest integer to test is floor((n-1)/3). */
    idx_t limit = (n - 1) / 3;

    for (idx_t k = 1; k <= limit; ++k) {
        if (gcd((seq_t)k, seq_n) == 1)
            ++count;
    }

    return count;
}
