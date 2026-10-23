/*
 * A108872  Sums of ordinal references for a triangular table read by columns,
 *          top to bottom.
 * https://oeis.org/A108872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = i + j, * where i is the smallest integer with i*(i+1)/2 ≥
 *            n * and j = n – i*(i-1)/2. (These are the * row and column of
 *            the n-th entry in the * triangular table read by rows, with i
 *            the * row index.)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   tri_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tri_uint64.h"

seq_t a(idx_t n) {
    /* Find smallest i such that tri_uint64(i) >= n. */
    uint64_t low = 1;
    uint64_t high = 1;

    /* Grow high until it covers n. */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high == 0) /* overflow guard for max uint64_t */
            break;
    }

    /* Binary search for the minimal i. */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (tri_uint64(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t i = low;

    /* j = n - (i-1)*i/2 */
    uint64_t j = n - tri_uint64(i - 1);

    /* Return i + j as seq_t. */
    return (seq_t)i + (seq_t)j;
}
