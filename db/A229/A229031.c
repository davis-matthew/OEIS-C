/*
 * A229031  Number of 5-colorings of the strong product of the complete graph
 *          K2 and the cycle graph Cn.
 * https://oeis.org/A229031
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 4*(-4)^n + 5*2^n (for n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Compute powers needed for the closed form */
    seq_t pow6 = seq_pow((seq_t)6, n);
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* 4*(-4)^n = (-1)^n * 4^(n+1) */
    seq_t term4 = seq_mul(pow4, (seq_t)4);          /* 4^(n+1)          */
    seq_t fivePow2 = seq_mul(pow2, (seq_t)5);       /* 5 * 2^n          */

    seq_t res = pow6;
    res += fivePow2;                               /* 6^n + 5*2^n      */

    /* Add or subtract the 4*(-4)^n term depending on parity of n */
    if (n & 1ULL)      /* n odd: subtract term4   */
        res -= term4;
    else               /* n even: add term4      */
        res += term4;

    return res;
}
