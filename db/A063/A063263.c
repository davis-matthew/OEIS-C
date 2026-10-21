/*
 * A063263  Ninth column (k=8) of sextinomial array A063260.
 * https://oeis.org/A063263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n^5 + 38*n^4 + 587*n^3 + 4678*n^2 +
 *            19896*n + 20160) / 8!
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 54, u64 n <= 958, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit arithmetic for intermediate values */
typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    /* Use u128 for all intermediate computations to avoid overflow. */
    u128 nn  = n;                 /* n                */
    u128 n2  = nn * nn;           /* n²               */
    u128 n3  = n2 * nn;           /* n³               */
    u128 n4  = n3 * nn;           /* n⁴               */
    u128 n5  = n4 * nn;           /* n⁵               */

    /* Polynomial part: n⁵ + 38*n⁴ + 587*n³ + 4678*n² + 19896*n + 20160 */
    u128 poly = n5
              + (u128)38   * n4
              + (u128)587  * n3
              + (u128)4678 * n2
              + (u128)19896 * nn
              + 20160;

    /* Factor (n+1)(n+2)(n+3) */
    u128 term = (nn + 1) * (nn + 2) * (nn + 3);

    /* Multiply, divide by 8! (=40320) and cast back to seq_t */
    u128 prod = term * poly;
    prod /= 40320;            /* 8! */

    return (seq_t)prod;
}
