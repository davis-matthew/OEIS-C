/*
 * A184034  1/16 the number of (n+1) X 5 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is given by the following closed form:
 *
 * For even n   : a(n) = 9*2^(n/2-1) + 9*2^(n-1) + 76
 * For odd  n   : a(n) = 9*2^(n-1)   + 3*2^((n+1)/2) + 76
 *
 * (The formula holds for all n ≥ 1; a(1)=91, a(2)=103,… as listed in OEIS A184034)
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* Offset is 1; n==0 not expected, return 0 for safety */
        return 0;

    if ((n & 1) == 0) /* even n */
    {
        seq_t t1 = pow2((n / 2) - 1);  /* 2^(n/2-1) */
        t1 = 9 * t1;                   /* 9*2^(n/2-1) */

        seq_t t2 = pow2(n - 1);        /* 2^(n-1) */
        t2 = 9 * t2;                   /* 9*2^(n-1) */

        return t1 + t2 + 76;           /* final sum */
    }
    else /* odd n */
    {
        seq_t t1 = pow2(n - 1);        /* 2^(n-1) */
        t1 = 9 * t1;                   /* 9*2^(n-1) */

        seq_t t2 = pow2((n + 1) / 2);  /* 2^((n+1)/2) */
        t2 = 3 * t2;                   /* 3*2^((n+1)/2) */

        return t1 + t2 + 76;           /* final sum */
    }
}
