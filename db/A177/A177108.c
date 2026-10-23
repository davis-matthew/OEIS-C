/*
 * A177108  a(n) = 4*(10^n-3).
 * https://oeis.org/A177108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(10^n-3) (offset : 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, but return 0 for safety */

    /* Compute 4*(10^n) - 12 */
    seq_t t   = pow10(n);                 /* 10^n  */
    seq_t res = t * (seq_t)4;             /* 4 * 10^n */
    res -= (seq_t)12;                    /* subtract 12 (4*3) */
    return res;
}
