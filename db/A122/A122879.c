/*
 * A122879  Periodic sequence of period 21.
 * https://oeis.org/A122879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+21) = a(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t seq[21] = {
    0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2,
    0, 0, 0, 1, 1, 1, 1, 2, 2, 2
};

seq_t a(idx_t n) {
    /* n is at least 1 */
    size_t idx = (size_t)((n - 1) % 21);
    return seq[idx];
}
