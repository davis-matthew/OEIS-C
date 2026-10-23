/*
 * A126234  a(0)=0; a(n)=a(n-1)-n, if a(n-1)>2n; otherwise a(n)=a(n-1)+n.
 * https://oeis.org/A126234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A126234: 
   a(0) = 0; 
   For n > 0, a(n) = a(n-1) - n   if a(n-1) > 2*n 
                    a(n-1) + n   otherwise
*/

#include <stdint.h>

/* seq_t and idx_t are typedefd by the build environment */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)0;          /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute threshold 2*i as seq_t */
        seq_t thr = (seq_t)2 * (seq_t)i;
        if (val > thr)
            val = val - (seq_t)i;
        else
            val = val + (seq_t)i;
    }
    return val;
}
