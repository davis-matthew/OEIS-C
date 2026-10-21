/*
 * A213120  Number of binary arrays of length 2*n+2 with fewer than n ones in
 *          any length 2n subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1) - (15*n-8)*C(2*n-2,n-1)/n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset is 1; n is guaranteed >= 1 when this function is called. */
    idx_t exp = 2 * n + 1;
    seq_t pow_val = pow2(exp);                       /* 2^(2*n+1) */

    seq_t bin = binomial(2 * n - 2, n - 1);          /* C(2*n-2, n-1) */
    seq_t y   = bin / (seq_t)n;                      /* bin / n   (integer) */

    seq_t coeff = (seq_t)15 * (seq_t)n - (seq_t)8;   /* 15*n-8   */
    seq_t term  = coeff * y;                         /* (15*n-8)*y */

    return pow_val - term;                           /* required value */
}
