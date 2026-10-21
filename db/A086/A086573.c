/*
 * A086573  a(n) = 2*(10^n - 1).
 * https://oeis.org/A086573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2 * (10^n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^n */
    seq_t power = (seq_t)1;
    const seq_t TEN = (seq_t)10;
    for (idx_t i = 0; i < n; ++i) {
        power *= TEN;
    }
    /* Return 2*(10^n - 1) */
    return power - (seq_t)1 + (power - (seq_t)1);
}
