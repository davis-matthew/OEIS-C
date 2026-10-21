/*
 * A064747  a(n) = n*9^n + 1.
 * https://oeis.org/A064747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*9^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 9^n */
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow9 *= (seq_t)9;
    }
    /* compute n * 9^n + 1 */
    return pow9 * (seq_t)n + (seq_t)1;
}
