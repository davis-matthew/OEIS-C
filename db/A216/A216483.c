/*
 * A216483  a(n) = Sum_{k=0..n} binomial(n,k)^3 * 4^k.
 * https://oeis.org/A216483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * 4^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;
    seq_t pow4  = 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom^3 * 4^k */
        seq_t term = binom * binom;
        term = term * binom;
        term = term * pow4;
        sum = sum + term;

        if (k == n) break;   /* no more terms */

        /* batch update for next k:
           binom_{k+1} = binom_k * (n-k) / (k+1)
           pow4_{k+1}  = pow4_k * 4
        */
        seq_t tmp = binom * ((seq_t)(n - k));
        binom = tmp / ((seq_t)(k + 1));
        pow4  = pow4 * (seq_t)4;
    }

    return sum;
}
