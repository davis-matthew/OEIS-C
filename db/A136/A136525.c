/*
 * A136525  a(n) = (2^n + 1)*(2^n + n + 1)^(n-1).
 * https://oeis.org/A136525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 1) * (2^n + n + 1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t twopow = pow2(n);                     /* 2^n                */
    seq_t term1  = twopow + (seq_t)1;           /* 2^n + 1            */
    seq_t base2  = twopow + (seq_t)n + (seq_t)1;/* 2^n + n + 1        */
    seq_t exp    = (seq_t)(n - 1);              /* n-1 (as seq_t)     */

    seq_t term2 = big_pow(base2, exp);          /* (2^n + n + 1)^(n-1)*/
    return term1 * term2;
}
