/*
 * A047462  Numbers that are congruent to {0, 1, 4} mod 8.
 * https://oeis.org/A047462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/3) + {0,1,4}[ (n-1)%3 ] (n ≥ 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is unsigned so n-1 is safe for n>0 */
    idx_t q = (n - 1) / 3;   /* quotient (block index)    */
    idx_t r = (n - 1) % 3;   /* remainder (position in block) */

    seq_t base = (seq_t)q * 8;   /* 8 * q in seq_t */

    switch (r) {
        case 0: return base;          /* 8*q          */
        case 1: return base + 1;      /* 8*q + 1      */
        case 2: return base + 4;      /* 8*q + 4      */
        default: return base;         /* unreachable */
    }
}
