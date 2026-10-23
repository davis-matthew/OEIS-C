/*
 * A151821  Powers of 2, omitting 2 itself.
 * https://oeis.org/A151821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A151821: Powers of 2, omitting 2 itself.  
   a(1) = 1; a(n) = 2^n for n ≥ 2.
   (The standard recurrence a(1)=1, a(n)=3+∑_{k=1}^{n-1}a(k) is equivalent.)
*/

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    else
        return pow2(n);
}
