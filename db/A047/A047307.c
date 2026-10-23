/*
 * A047307  Numbers that are congruent to {3, 4, 5, 6} mod 7.
 * https://oeis.org/A047307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + ((n-1)%4) + 3
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047307: numbers congruent to {3,4,5,6} (mod 7) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n < 1 the value is
       undefined; return 0 to avoid underflow. */
    if (n < 1)
        return 0;

    idx_t n1 = n - 1;
    idx_t q = n1 / 4;   /* number of complete 7‑item blocks before n  */
    idx_t r = n1 % 4;   /* position within the current block (0..3)   */

    /* Map r to the allowed residue: r = 0→3, 1→4, 2→5, 3→6.
       The final value is 7*q + (r+3). */
    return (seq_t)q * (seq_t)7 + (seq_t)(r + 3);
}
