/*
 * A068156  G.f.: (x+2)*(x+1)/((x-1)*(x-2)) = Sum_{n>=0} a(n)*(x/2)^n.
 * https://oeis.org/A068156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 0^n - 3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. a(0)=1, a(n)=3*2^n-3 for n>0) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t v = pow2(n);   /* v = 2^n */
    v = v * 3;           /* v = 3 * 2^n */
    v = v - 3;           /* v = 3*2^n - 3 */
    return v;
}
