/*
 * A236208  Numbers not divisible by 2, 5 or 11.
 * https://oeis.org/A236208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer coprime to 110 (i.e., not
 *            divisible by 2, 5 or 11). The value is found by binary searching
 *            the inclusion–exclusion count of numbers ≤ x that are coprime to
 *            110
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   coprime110_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/coprime110_count.h"

seq_t a(idx_t n)
{
    /* Assume n ≥ 1.  For safety, set an upper bound using the density
       4/11: a(n) ≤ ceil(11*n/4).  Add a small padding. */
    idx_t high = (n * 11) / 4 + 100;
    idx_t low = 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (coprime110_count(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
