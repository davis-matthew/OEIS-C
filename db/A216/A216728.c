/*
 * A216728  (2*n)!*((2*n+1)/2)^n/(2*n+1).
 * https://oeis.org/A216728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)! * ((2*n+1)/2)^n)/(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t fac   = factorial(2 * n);                     /* (2n)!          */
    seq_t powf  = seq_pow((seq_t)(2 * n + 1), n - 1);    /* (2n+1)^(n-1)   */
    seq_t denom = pow2(n);                               /* 2^n            */
    return (fac * powf) / denom;
}
