/*
 * A061879  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 10.
 * https://oeis.org/A061879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 2328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_sum_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061879: Numbers whose alternating digit sum equals 10. */

#include <stdint.h>
#include "../utils/alt_sum_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence starts at offset 1 */

    idx_t count = 0;
    idx_t num   = 1;

    for (;; ++num) {
        if (alt_sum_idx(num) == 10) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
