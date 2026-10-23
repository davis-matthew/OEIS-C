/*
 * A187018  Coefficient of x^n in (1 + x + n*x^2)^n.
 * https://oeis.org/A187018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n-k,k) *
 *            n^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Edge cases are handled naturally by the loop. */
    idx_t m = n >> 1;        /* floor(n/2) */

    seq_t ck = 1;            /* C(n, k)  */
    seq_t bk = 1;            /* C(n-k, k) */
    seq_t power_n = 1;       /* n^k          */
    seq_t sum = 0;

    for (idx_t k = 0; k <= m; ++k)
    {
        /* term = ck * bk * power_n */
        seq_t term = ck * bk;
        term = term * power_n;
        sum = sum + term;

        /* Prepare for next k (if any) */
        if (k == m)
            break;

        /* Update ck = C(n, k+1) */
        seq_t num_ck = (seq_t)(n - k);
        seq_t den_ck = (seq_t)(k + 1);
        ck = ck * num_ck / den_ck;

        /* Update bk = C(n-(k+1), k+1) */
        seq_t num1 = (seq_t)(n - 2 * k - 1);
        seq_t num2 = (seq_t)(n - 2 * k);
        seq_t den1 = (seq_t)(n - k);
        seq_t den2 = (seq_t)(k + 1);
        seq_t temp   = bk * num1 * num2;   /* (bk * (n-2k-1) * (n-2k)) */
        seq_t denom  = den1 * den2;        /* (n-k)*(k+1)               */
        bk = temp / denom;

        /* Update power_n = n^(k+1) */
        power_n = power_n * n;
    }

    return sum;
}
