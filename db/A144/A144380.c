/*
 * A144380  Third subdiagonal of A142458: a(n) = A142458(n+3,n).
 * https://oeis.org/A144380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/162)*( 8*10^(n+3) - 30*(3*n +8)*7^(n+2) + 6*(9*n^2
 *            +39*n +40)*4^(n+2) - (27*n^3 +135*n^2 +198*n +80))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses 128‑bit arithmetic when seq_t is 64‑bit,
   to avoid overflow in the intermediate powers that exceed 64 bits.
   For larger integer types it uses the supplied pow_uint routine
   and seq_t arithmetic. */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp); */

/* ---------  128‑bit power helper  --------- */
static inline __int128 pow_uint128(unsigned base, idx_t exp)
{
    __int128 result = 1;
    __int128 b = base;
    while (exp)
    {
        if (exp & 1)
            result *= b;
        if (exp > 1)
            b *= b;
        exp >>= 1;
    }
    return result;
}

/* ---------  main function  --------- */
seq_t a(idx_t n)
{
    /* a(1) is the first term; underflow for n < 1 is undefined */
    if (n < 1)
        return 0;

    /* 64‑bit integer implementation: use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8)   /* assume seq_t == uint8_t/uint16_t/uint32_t/uint64_t */
    {
        __int128 pow10 = pow_uint128(10, n + 3);
        __int128 pow7  = pow_uint128(7 , n + 2);
        __int128 pow4  = pow_uint128(4 , n + 2);

        __int128 poly1 = 3 * (__int128)n + 8;
        __int128 poly2 = 9 * (__int128)n * n + 39 * (__int128)n + 40;
        __int128 poly3 = 27 * (__int128)n * n * n + 135 * (__int128)n * n
                         + 198 * (__int128)n + 80;

        __int128 num = 8 * pow10 - 30 * poly1 * pow7
                       + 6 * poly2 * pow4 - poly3;

        __int128 res = num / 162;
        return (seq_t)res;          /* fits into seq_t by problem statement */
    }
    else
    {   /* arbitrary‑precision implementation */
        seq_t pow10 = pow_uint(10, n + 3);
        seq_t pow7  = pow_uint(7 , n + 2);
        seq_t pow4  = pow_uint(4 , n + 2);

        seq_t poly1 = (seq_t)(3 * n + 8);
        seq_t poly2 = (seq_t)(9 * n * n + 39 * n + 40);
        seq_t poly3 = (seq_t)(27 * n * n * n + 135 * n * n
                              + 198 * n + 80);

        seq_t num = 8 * pow10;
        num = num - (30 * poly1 * pow7);
        num = num + (6 * poly2 * pow4);
        num = num - poly3;

        seq_t res = num / 162;
        return res;
    }
}
