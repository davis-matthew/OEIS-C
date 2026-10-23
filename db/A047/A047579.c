/*
 * A047579  Numbers that are congruent to {0, 2, 5, 6, 7} mod 8.
 * https://oeis.org/A047579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,2,5,6,7}[ (n-1) % 5 ] This gives
 *            the nth integer congruent to 0, 2, 5, 6, or 7 modulo 8
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the case n==0 if called (undefined behavior is fine). */
    if (n == 0) return 0;  /* optional; will not be used for valid input */

    static const seq_t map[5] = {0, 2, 5, 6, 7};
    idx_t q = (n - 1) / 5;          /* number of complete blocks of 5 terms */
    idx_t r = (n - 1) % 5;          /* position within the current block   */
    seq_t base = (seq_t)q * 8;      /* 5 terms → +8 each block */

    return base + map[r];
}
