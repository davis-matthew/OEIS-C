/*
 * A047424  Numbers that are congruent to {0, 2, 3, 4, 5, 6} mod 8.
 * https://oeis.org/A047424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A047424: Numbers congruent to {0,2,3,4,5,6} mod 8. 
   For n >= 1 let k = (n + 5) / 6 (integer division) and r = n % 6.  
   Then: 
        r = 0 : a(n) = 8*k - 2
        r = 1 : a(n) = 8*k - 8
        r = 2 : a(n) = 8*k - 6
        r = 3 : a(n) = 8*k - 5
        r = 4 : a(n) = 8*k - 4
        r = 5 : a(n) = 8*k - 3
   This reproduces the sequence 0,2,3,4,5,6,8,10,...
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n starts at 1 */
    idx_t k = (n + 5) / 6;
    idx_t r = n % 6;

    seq_t nk = (seq_t)k * (seq_t)8;

    switch (r)
    {
        case 0: return nk - (seq_t)2;
        case 1: return nk - (seq_t)8;
        case 2: return nk - (seq_t)6;
        case 3: return nk - (seq_t)5;
        case 4: return nk - (seq_t)4;
        case 5: return nk - (seq_t)3;
        default: return (seq_t)0;   /* unreachable, but keeps compiler happy */
    }
}
