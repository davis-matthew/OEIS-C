/*
 * A224461  The Wiener index of Wang's helicene-based dendrimer defined
 *          pictorially as DNS_2[n] in the Shabani et al. reference.
 * https://oeis.org/A224461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 335 + 2^n*(6858*n+37319/2)+4^n*(6561*n-37989/2)
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset is 1 */

    seq_t result = (seq_t)335;

    /* powers of 2 and 4 */
    seq_t two_n          = pow2((idx_t)n);
    seq_t two_n_minus1   = pow2((idx_t)(n - 1));
    seq_t four_n         = pow2((idx_t)(2 * n));
    seq_t four_n_minus1  = pow2((idx_t)(2 * n - 2));

    /* term 6858*n*2^n */
    result += (seq_t)6858 * (seq_t)n * two_n;
    /* term 37319*2^(n-1) */
    result += (seq_t)37319 * two_n_minus1;
    /* term 6561*n*4^n */
    result += (seq_t)6561 * (seq_t)n * four_n;
    /* subtract (37989/2)*4^n = 75978*4^(n-1) */
    result -= (seq_t)75978 * four_n_minus1;

    return result;
}
