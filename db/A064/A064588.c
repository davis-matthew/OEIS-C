/*
 * A064588  a(n) = (2^n)^12*((2^n)^2-1)*((2^n)^6-1)*((2^n)^8 + (2^n)^4+1).
 * https://oeis.org/A064588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n)^12 * ((2^n)^2-1) * ((2^n)^6-1) * ((2^n)^8 + (2^n)^4
 *            + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute powers of two with multiplied exponents */
    seq_t t12 = pow2((idx_t)12 * n);   /* 2^(12n) */
    seq_t t2n  = pow2((idx_t)2  * n);  /* 2^( 2n) */
    seq_t t6n  = pow2((idx_t)6  * n);  /* 2^( 6n) */
    seq_t t8n  = pow2((idx_t)8  * n);  /* 2^( 8n) */
    seq_t t4n  = pow2((idx_t)4  * n);  /* 2^( 4n) */

    /* factors (t^k - 1) and (t^8 + t^4 + 1) */
    seq_t B = t2n - 1;                /* 2^(2n) - 1 */
    seq_t C = t6n - 1;                /* 2^(6n) - 1 */
    seq_t D = t8n + t4n + 1;          /* 2^(8n) + 2^(4n) + 1 */

    /* final product */
    seq_t res = t12 * B * C * D;
    return res;
}
