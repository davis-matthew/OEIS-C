/*
 * A235151  Numbers whose sum of digits is 12.
 * https://oeis.org/A235151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose decimal digit sum is 12
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_seqT
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum_seqT.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0, but avoid UB */

    seq_t current = 0;
    idx_t count   = 0;

    while (count < n) {
        ++current;                   /* next integer to test */
        if (digit_sum_seqT(current) == 12) {
            ++count;                 /* found the nth term */
        }
    }

    return current;
}
