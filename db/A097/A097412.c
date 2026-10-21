/*
 * A097412  Initial decimal digit of n^8.
 * https://oeis.org/A097412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the leading decimal digit of n^8
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   pow10_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_uint128.h"

/* Leading decimal digit of n^8 */
seq_t a(idx_t n)
{
    if (n == 0)           /* 0^8 = 0, leading digit 0 */
        return (seq_t)0;

    /* Compute n^8 using 128‑bit arithmetic */
    __uint128_t x  = (__uint128_t)n;          /* n         */
    __uint128_t n2 = x * x;                  /* n^2       */
    __uint128_t n4 = n2 * n2;                /* n^4       */
    __uint128_t n8 = n4 * n4;                /* n^8       */

    /* Determine the number of decimal digits of n8 */
    int len = 0;
    __uint128_t tmp = n8;
    while (tmp > 0) {
        tmp /= 10;
        ++len;
    }

    /* Leading digit = n8 / 10^(len−1) */
    __uint128_t pow10val = pow10_uint128(len - 1);
    unsigned long long lead = (unsigned long long)(n8 / pow10val);

    return (seq_t)lead;      /* lead is between 0 and 9 */
}
