/*
 * A047326  Numbers that are congruent to {0, 1, 2, 5, 6} mod 7.
 * https://oeis.org/A047326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 * floor((n-1)/5) + offset[(n-1)%5] where offset = {0,
 *            1, 2, 5, 6} and a(1) = 0
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t offset[5] = {0, 1, 2, 5, 6};

    /* n >= 1, so n-1 is safe */
    idx_t nm1 = n - 1;
    seq_t q   = ((seq_t)nm1) / 5;
    idx_t r   = nm1 % 5;

    return (seq_t)7 * q + (seq_t)offset[r];
}
