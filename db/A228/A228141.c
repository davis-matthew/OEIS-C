/*
 * A228141  Numbers that are congruent to {1, 5} mod 20.
 * https://oeis.org/A228141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*(4+(-1)^n) + 10*n (offset 1)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 0, return 0 */

    idx_t half_idx = (n - 1) / 2;  /* floor((n-1)/2) */
    seq_t term = ((seq_t)20 * (seq_t)half_idx) + ((n & 1) ? 1 : 5);
    return term;
}
