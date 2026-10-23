/*
 * A047316  Numbers that are congruent to {0, 2, 4, 5, 6} mod 7.
 * https://oeis.org/A047316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 * floor((n-1)/5) + {0,2,4,5,6}[(n-1) mod 5]
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series of nonnegative integers ≠ 1,3 (mod 7), offset 1 */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t off[5] = {0, 2, 4, 5, 6};
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    return ((seq_t)7 * (seq_t)k) + off[(size_t)r];
}
