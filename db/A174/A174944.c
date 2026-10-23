/*
 * A174944  Greatest number k such that sum of the n-th powers of the digits
 *          of k is greater than k.
 * https://oeis.org/A174944
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum integer k such that the sum of the n-th powers of
 *            the digits of k is greater than k
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n ≥ 2 */

    /* Precompute 9^n and d^n for d = 1..9. */
    seq_t pow9 = pow_uint(9, n);

    seq_t pow_d[10];
    pow_d[0] = 0;
    pow_d[1] = 1;                 /* 1^n = 1 */
    for (int d = 2; d <= 9; ++d)
        pow_d[d] = pow_uint(d, n);

    seq_t sum9 = 0;                /* running sum of c * 9^n */
    seq_t pow10 = 1;               /* 10^c, starting with 10^0 */
    seq_t best_k = 0;              /* answer for current n */

    /* Iterate over number of trailing 9's: c = 1,2,3,... */
    for (idx_t c = 1;; ++c)
    {
        /* sum9 = c * 9^n */
        sum9 += pow9;

        /* pow10 = 10^c */
        pow10 *= 10;

        /* Find the largest leading digit d (1..9) that satisfies the inequality. */
        int best_d = -1;
        for (int d = 9; d >= 1; --d)
        {
            seq_t lhs = pow_d[d] + sum9;                     /* d^n + c*9^n */
            seq_t rhs = pow10 * (seq_t)(d + 1) - 1;          /* (d+1)*10^c - 1 (=k) */
            if (lhs > rhs)
            {
                best_d = d;
                break;   /* largest d found for this c */
            }
        }

        if (best_d == -1)            /* no k exists for this c, larger c will also fail */
            break;
        else
        {
            /* compute the candidate k for this c and best_d */
            seq_t k = pow10 * (seq_t)(best_d + 1) - 1;
            if (k > best_k)
                best_k = k;
        }
    }

    return best_k;
}
