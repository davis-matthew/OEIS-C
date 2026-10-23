/*
 * A040445  Continued fraction for sqrt(467).
 * https://oeis.org/A040445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continued fraction for sqrt(467); a(0)=21, for n>=1:
 *            a(n)=p[(n-1)%14] where p = {1,1,1,1,3,3,21,3,3,1,1,1,1,42}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)21;
    /* period of the continued fraction of sqrt(467) */
    static const seq_t p[14] = {
        1, 1, 1, 1,  /* 1,1,1,1   */
        3, 3,       /* 3,3       */
        21,         /* 21        */
        3, 3,       /* 3,3       */
        1, 1, 1, 1, /* 1,1,1,1   */
        42          /* 42        */
    };
    idx_t idx = (n - 1) % 14;
    return p[(size_t)idx];
}
