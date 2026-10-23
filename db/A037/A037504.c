/*
 * A037504  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) + d_{n-1}, a(1)=1, where d_k follows the
 *            periodic sequence 1,2,0 (k=0,1,2,3,…)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, but safe */
    seq_t value = 1;                            /* a(1) */
    idx_t i = 1;                                /* current number of digits */
    static const seq_t pattern[3] = {1, 2, 0};   /* cycle 1,2,0 */

    while (i < n) {
        value = value * 3 + pattern[i % 3];
        ++i;
    }
    return value;
}
