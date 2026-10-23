/*
 * A047591  Numbers that are congruent to {1, 6, 7} mod 8.
 * https://oeis.org/A047591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-1)/3⌋ + {1,6,7}_{(n-1) mod 3}, n≥1. Numbers
 *            congruent to 1, 6 or 7 modulo 8
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* the sequence is defined for n≥1 */

    idx_t q = (n - 1) / 3;             /* quotient */
    idx_t r = (n - 1) % 3;             /* remainder 0,1,2 */

    static const seq_t off[3] = {1, 6, 7};
    return (seq_t)(8 * q + off[(size_t)r]);
}
