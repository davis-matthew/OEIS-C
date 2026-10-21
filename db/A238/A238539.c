/*
 * A238539  A fourth-order linear divisibility sequence: a(n) = (1/9)*(2^n +
 *          (-1)^n)*(2^(3*n) - (-1)^n).
 * https://oeis.org/A238539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/9) * (2^n + (-1)^n) * (2^(3n) - (-1)^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the second factor divided by 9 first
 * to avoid overflow on types smaller than arbitrary‑precision.      */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute powers of two */
    seq_t two_n   = pow2(n);        /* 2^n        */
    seq_t two_3n  = pow2(3 * n);    /* 2^(3n)     */

    seq_t term1, term2_div9;

    if (n & 1U)  /* n is odd: (-1)^n = -1 */
    {
        term1        = two_n - 1;                 /* 2^n - 1               */
        term2_div9    = (two_3n + 1) / 9;          /* (2^(3n)+1)/9           */
    }
    else        /* n is even: (-1)^n = +1 */
    {
        term1        = two_n + 1;                 /* 2^n + 1               */
        term2_div9    = (two_3n - 1) / 9;          /* (2^(3n)-1)/9           */
    }

    return term1 * term2_div9; /* exact division guaranteed by the formula */
}
