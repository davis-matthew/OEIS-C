/*
 * A151638  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 8 adjacent element pairs in decreasing order.
 * https://oeis.org/A151638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..10} (-1)^j*binomial(3*n+1,
 *            10-j)*(binomial(j+1,3))^n
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t total = 3*n + 1;
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (idx_t j = 0; j <= 10; ++j) {
        idx_t k = 10 - j;
        seq_t choose1 = binomial(total, k);               /* binomial(3*n+1, 10-j) */
        seq_t choose2 = binomial(j + 1, 3);                /* binomial(j+1,3)   */
        seq_t power   = big_pow(choose2, (seq_t)n);        /* (choose2)^n       */
        seq_t term    = choose1 * power;                   /* term of summand   */

        if ((j & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;
    }

    return sum_even - sum_odd; /* result is always non‑negative */
}
