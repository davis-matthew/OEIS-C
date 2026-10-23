/*
 * A123208  Start with 1, then alternately add 2 or double.
 * https://oeis.org/A123208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 5*2^n - 4 * Formula: a(2n+1) = 5*2^n - 2 * (indices
 *            start at 0)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {                 /* n even  -> a(2n) */
        seq_t t = pow2(n / 2);             /* 2^n            */
        t *= (seq_t)5;                     /* 5*2^n          */
        t -= (seq_t)4;                     /* 5*2^n - 4      */
        return t;
    } else {                                 /* n odd   -> a(2n+1) */
        seq_t t = pow2((n - 1) / 2);        /* 2^n            */
        t *= (seq_t)5;                     /* 5*2^n          */
        t -= (seq_t)2;                     /* 5*2^n - 2      */
        return t;
    }
}
