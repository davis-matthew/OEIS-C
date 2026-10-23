/*
 * A179370  Positive integers of the form (7*m^2+1)/11.
 * https://oeis.org/A179370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7 * ((11*(2*n-1) - 9*(-1)^n)/4)^2 + 1) / 11 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset, but guard against 0 */
        return (seq_t)0;

    /* 128‑bit intermediates guarantee no overflow for n up to ~10^4
       (the performance targets of this problem).  */
    __uint128_t n128 = n;
    __uint128_t t   = 22 * n128 - 11;          /* 22*n - 11                */
    if (n & 1ULL)      /* n is odd => (-1)^n = -1  */
        t += 9;        /* 22*n - 11 + 9             */
    else               /* n is even => (-1)^n = 1  */
        t -= 9;        /* 22*n - 11 - 9             */

    __uint128_t m   = t / 4;                   /* integer m                */
    __uint128_t res = (7 * m * m + 1) / 11;    /* a(n)                      */
    return (seq_t)res;
}
