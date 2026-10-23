/*
 * A048291  Number of {0,1} n X n matrices with no zero rows or columns.
 * https://oeis.org/A048291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k) * (2^{n-k} - 1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n=0 the loop below also works, but we handle it explicitly
       for clarity and to avoid any edge case in pow2(0). */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t comb   = binomial(n, k);    /* C(n,k) */
        idx_t t      = n - k;             /* exponent for 2 */
        seq_t two   = pow2(t);            /* 2^t  */
        seq_t base  = two;
        base -= (seq_t)1;                 /* 2^t - 1 */

        /* (2^t - 1)^n */
        seq_t pow_term = big_pow(base, (seq_t)n);

        /* term = C(n,k) * (2^t - 1)^n */
        seq_t term = comb * pow_term;

        if (k & 1) {
            sum_neg += term;
        } else {
            sum_pos += term;
        }
    }

    /* result = sum_pos - sum_neg; the subtraction is safe because
       sum_pos >= sum_neg for all n. */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
