/*
 * A083015  a(n) = the number of ordered pairs of n-bit bit strings, such that
 *          both bit strings have at least n/2 one-bits, or both have at most
 *          n/2 one-bits.
 * https://oeis.org/A083015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - C(n,n/2)^2)/2 + 2^n*C(n,n/2), where C(n,n/2)=0
 *            when n is odd
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* 2^n and 4^n */
    seq_t two_n      = pow2(n);          /* 2^n          */
    seq_t four_pow_n = pow2(2 * n);      /* 2^(2n) = 4^n */

    /* central binomial coefficient, or 0 if n is odd */
    seq_t c = 0;
    if ((n & 1) == 0)
        c = binom(n, n / 2);

    /* (4^n - c^2)/2  */
    seq_t half = (four_pow_n - c * c) / 2;

    /* 2^n * c      */
    seq_t add = two_n * c;

    return half + add;
}
