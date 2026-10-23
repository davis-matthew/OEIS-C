/*
 * A043528  Numbers having four 9's in base 10.
 * https://oeis.org/A043528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th (positive) integer whose decimal representation
 *            contains exactly four digits equal to 9
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_4nines_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/count_4nines_limit.h"

seq_t a(idx_t n)
{
    /* Find a high bound that is guaranteed to contain at least n numbers. */
    uint64_t high = 1;
    while (count_4nines_limit(high, n) <= n) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    uint64_t low = 0;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (count_4nines_limit(mid, n) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
