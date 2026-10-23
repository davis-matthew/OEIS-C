/*
 * A133818  a(n) = (8*n+3)*(8*n+5)*(8*n+7)*(8*n+9).
 * https://oeis.org/A133818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)*(8*n+5)*(8*n+7)*(8*n+9)
 * Range:     u8 none, u16 n <= 1, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for safety */
    __uint128_t m = ( __uint128_t )n;
    __uint128_t t = 8 * m + 3;
    t *= 8 * m + 5;
    t *= 8 * m + 7;
    t *= 8 * m + 9;
    return (seq_t)t;
}
