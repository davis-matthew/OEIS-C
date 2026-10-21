/*
 * A081298  Main diagonal of the square array A081297.
 * https://oeis.org/A081298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+1)^(n+1)-(-n)^(n+1))/(2n+1) (OEIS A081298)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* bases and exponent */
    seq_t base1 = (seq_t)(n + 1);      /* n+1  */
    seq_t base2 = (seq_t)(n);          /* n    */
    seq_t exp   = (seq_t)(n + 1);      /* n+1  */

    /* compute (n+1)^(n+1)  and  n^(n+1) */
    seq_t pow1 = big_pow(base1, exp);
    seq_t pow2 = big_pow(base2, exp);

    /* numerator: ((n+1)^(n+1) - (-n)^(n+1)) */
    seq_t numer;
    if ((n + 1) & 1) {               /* exponent odd  =>  (-n)^(n+1) = -n^(n+1) */
        numer = pow1 + pow2;          /* subtraction of a negative = addition          */
    } else {                          /* exponent even =>  (-n)^(n+1) =  n^(n+1)  */
        numer = pow1 - pow2;          /* normal subtraction                           */
    }

    /* denominator 2*n + 1, cast to seq_t */
    seq_t denom = (seq_t)(2 * n + 1);

    /* return exact division */
    return numer / denom;
}
