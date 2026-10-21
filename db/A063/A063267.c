/*
 * A063267  Eighth column (k=7) of septinomial array A063265.
 * https://oeis.org/A063267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=2}^{7} binomial(n+k, n) = sum_{k=2}^{7}
 *            binomial(n+k, k) (only 6 terms, k ≤ 7, so compute each binomial
 *            multiplicatively)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t fact[8] = {
    1ULL,          /* 0!  */
    1ULL,          /* 1!  */
    2ULL,          /* 2!  */
    6ULL,          /* 3!  */
    24ULL,         /* 4!  */
    120ULL,        /* 5!  */
    720ULL,        /* 6!  */
    5040ULL        /* 7!  */
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* only 6 terms: k = 2 .. 7 */
    for (int k = 2; k <= 7; ++k) {
        seq_t prod = 1;
        /* compute (n+1)(n+2)...(n+k) */
        for (int i = 1; i <= k; ++i)
            prod *= (seq_t)(n + i);
        /* divide by k! */
        prod /= fact[k];
        sum += prod;
    }
    return sum;
}
