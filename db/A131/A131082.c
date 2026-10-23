/*
 * A131082  Periodic sequence (15, 11, 5, 1, 1, 5, 11, 15).
 * https://oeis.org/A131082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8), period 8 sequence (15, 11, 5, 1, 1, 5, 11, 15)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t period[8] = {15, 11, 5, 1, 1, 5, 11, 15};
    size_t idx = (size_t)((n - 1) % 8);
    return period[idx];
}
