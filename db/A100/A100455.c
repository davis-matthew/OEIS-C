/*
 * A100455  a(n) = 2^n + sin(n*Pi/2).
 * https://oeis.org/A100455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + sin(n*Pi/2) (sin pattern = 0,1,0,-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);           /* 2^n  */
    switch (n & 3) {            /* n mod 4 */
        case 0: return p;            /* sin = 0  */
        case 1: return p + 1;        /* sin = 1  */
        case 2: return p;            /* sin = 0  */
        case 3: return p - 1;        /* sin = -1 */
    }
    return p; /* unreachable */
}
