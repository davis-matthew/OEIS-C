/*
 * A154682  a(n) = (2n-1)^(2n+1) + (2n+1)^(2n-1).
 * https://oeis.org/A154682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)^(2n+1) + (2n+1)^(2n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t u       = (seq_t)n;     /* Convert index to seq_t */
    seq_t two_n   = u + u;        /* 2n  */
    seq_t l       = two_n - 1;    /* 2n-1 */
    seq_t r       = two_n + 1;    /* 2n+1 */
    seq_t exp_l   = two_n + 1;    /* 2n+1 */
    seq_t exp_r   = two_n - 1;    /* 2n-1 */

    seq_t term1 = big_pow(l, exp_l);
    seq_t term2 = big_pow(r, exp_r);

    return term1 + term2;
}
