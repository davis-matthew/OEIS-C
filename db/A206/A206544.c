/*
 * A206544  Period 12: repeat 1, 3, 5, 7, 9, 11, 11, 9, 7, 5, 3, 1.
 * https://oeis.org/A206544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * min( ((n-1) % 12) + 1, 12 - ((n-1) % 12) ) - 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = n modulo 12, where 0..11 */
    idx_t r = (n - 1) % 12;   /* r in [0,11] */

    idx_t r2;
    if (r < 6)
        r2 = r + 1;          /* values 1..6 */
    else
        r2 = 12 - r;         /* values 6..1 */

    return (seq_t)(2 * r2 - 1);
}
