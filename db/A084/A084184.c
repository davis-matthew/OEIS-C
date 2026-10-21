/*
 * A084184  Partial sums of a Jacobsthal related sequence.
 * https://oeis.org/A084184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (5-(-1)^n)*(2^n-1)/6
 * Simplified:
 *   if n is odd:  a(n) = 2^n - 1
 *   if n is even: a(n) = 2*(2^n - 1)/3
 * This implementation uses 128‑bit integer arithmetic for 64‑bit builds
 * to avoid overflow while keeping the result exact for all values
 * that fit in `seq_t`.  For larger `seq_t` sizes the supplied pow2()
 * helper is used.
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 64‑bit / 128‑bit builds: use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit left shift is defined as long as n < 128 */
        if (n >= 128) {
            /* n is so large that the result never fits in 64 bits.
               Returning 0 avoids undefined behavior and matches the
               fact that there is no value that can be represented. */
            return (seq_t)0;
        }
        __uint128_t two_n = (__uint128_t)1 << n;
        __uint128_t sub  = two_n - 1;          /* 2^n - 1 */
        if (n & 1) {                           /* odd n */
            return (seq_t)sub;
        }
        __uint128_t tmp = sub * 2;              /* 2*(2^n - 1) */
        tmp /= 3;                               /* divide by 3 exactly */
        return (seq_t)tmp;
    }

    /* Larger integer types (>=128 bits or arbitrary precision):
       use the pow2 helper and normal seq_t arithmetic. */
    seq_t pow = pow2(n);
    seq_t sub = pow - 1;                       /* 2^n - 1 */
    if (n & 1) {                               /* odd n */
        return sub;
    }
    seq_t tmp = sub * (seq_t)2;                /* 2*(2^n - 1) */
    tmp /= (seq_t)3;                           /* exact division */
    return tmp;
}
