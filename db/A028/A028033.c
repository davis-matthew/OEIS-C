/*
 * A028033  Expansion of 1/((1-3x)(1-4x)(1-6x)(1-8x)).
 * https://oeis.org/A028033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*8^(n+3)-10*6^(n+3)+15*4^(n+3)-8*3^(n+3))/120
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent e = n+3 (fits in idx_t) */
    idx_t e = n + 3;
    seq_t exp = (seq_t)e;          /* cast to the seq_t type used for exponents */

    seq_t pow3 = big_pow((seq_t)3, exp);
    seq_t pow4 = big_pow((seq_t)4, exp);
    seq_t pow6 = big_pow((seq_t)6, exp);
    seq_t pow8 = big_pow((seq_t)8, exp);

    seq_t t1 = pow8 * (seq_t)3;          /* 3 * 8^(n+3) */
    seq_t t2 = pow6 * (seq_t)10;         /* 10 * 6^(n+3) */
    seq_t t3 = pow4 * (seq_t)15;         /* 15 * 4^(n+3) */
    seq_t t4 = pow3 * (seq_t)8;          /* 8 * 3^(n+3) */

    seq_t sum1 = t1 + t3;                /* 3*8^(n+3) + 15*4^(n+3) */
    seq_t sum2 = t2 + t4;                /* 10*6^(n+3) + 8*3^(n+3) */
    seq_t diff = sum1 - sum2;            /* non‑negative difference */
    seq_t ans  = diff / (seq_t)120;      /* divide by 120 */

    return ans;
}
