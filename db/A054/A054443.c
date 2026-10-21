/*
 * A054443  Third convolution of A001405 (central binomial numbers).
 * https://oeis.org/A054443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*k) = (2*k+7)*4^(k+1) - binomial(2*(k+2), k+2)*(4*k+9)/2
 *            a(2*k+1) = (k+4)*4^(k+2) - (k+3)*binomial(2*(k+3), k+3) where k
 *            = floor(n/2) and binomial is the standard binomial coefficient
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 26, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute 4^exp as a seq_t. */
static seq_t pow4(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 4;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)                          /* n = 2*k  (even) */
    {
        idx_t k = n >> 1;
        seq_t pow   = pow4(k + 1);                            /* 4^(k+1) */
        seq_t term1 = ((seq_t)(2 * k + 7)) * pow;              /* (2k+7)*4^(k+1) */

        seq_t c = binom(2 * (k + 2), k + 2);                    /* C(2(k+2), k+2) */
        seq_t term2 = c * ((seq_t)(4 * k + 9));                 /* c*(4k+9) */
        term2 /= 2;                                             /* divide by 2 */

        return term1 - term2;
    }
    else                                            /* n = 2*k+1  (odd) */
    {
        idx_t k = n >> 1;                          /* floor(n/2) */
        seq_t pow   = pow4(k + 2);                            /* 4^(k+2) */
        seq_t term1 = ((seq_t)(k + 4)) * pow;                 /* (k+4)*4^(k+2) */

        seq_t c = binom(2 * (k + 3), k + 3);                    /* C(2(k+3), k+3) */
        seq_t term2 = ((seq_t)(k + 3)) * c;                     /* (k+3)*c */

        return term1 - term2;
    }
}
