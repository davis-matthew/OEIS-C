/*
 * A214546  First differences of A140472.
 * https://oeis.org/A214546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; For n>0: if n is even: a(n) = -(2^(v2(n)-1)-1) // v2(n)
 *            = exponent of 2 in n if n is odd : a(n) = 2^(t-1) // t = number
 *            of consecutive trailing 1-bits of n
 * Range:     i8 n <= 254, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* returns 2^exp as seq_t */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return 1;

    /* Even indices */
    if ((n & 1ULL) == 0) {
        /* The number of trailing zeros in n (v2(n)), n>0 */
        unsigned int v = __builtin_ctzll((unsigned long long)n); /* v ≥ 1 for even n */
        seq_t power = pow2(v - 1);          /* 2^(v-1) */
        seq_t val   = power - 1;            /* 2^(v-1) - 1 */
        return -val;                        /* negative value */
    }

    /* Odd indices – count trailing 1 bits */
    idx_t  tmp = n;
    unsigned int cnt = 0;
    while ((tmp & 1ULL) == 1) {
        ++cnt;
        tmp >>= 1;
    }
    /* cnt ≥ 1 because n is odd */
    seq_t power = pow2(cnt - 1);  /* 2^(cnt-1) */
    return power;
}
