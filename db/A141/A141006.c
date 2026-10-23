/*
 * A141006  E.g.f. exp(sum_{d|M} (exp(d*x)-1)/d), M=9.
 * https://oeis.org/A141006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n/n! in exp( (exp(x)-1) + (exp(3*x)-1)/3
 *            + (exp(9*x)-1)/9 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation: store already computed a(i) values. */
    static seq_t *memo = NULL;         /* array of a(i) */
    static size_t memo_cap = 0;        /* allocated size of memo */
    static size_t memo_len = 0;        /* number of values currently stored */

    /* Ensure capacity for indices up to n. */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Initial value a(0) = 1. */
    if (memo_len == 0) {
        memo[0] = 1;
        memo_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (size_t m = memo_len; m <= (size_t)n; ++m) {
        seq_t sum = 0;                      /* a(m) */
        seq_t pow3 = 1;                     /* 3^(k-1) during inner loop */
        seq_t pow9 = 1;                     /* 9^(k-1) during inner loop */
        seq_t binom = 1;                    /* C(m-1, k-1) */

        for (size_t k = 1; k <= m; ++k) {
            /* A(k) = 1 + 3^(k-1) + 9^(k-1) */
            seq_t Ak = pow3;
            Ak += pow9;
            Ak += 1;

            /* term = A(k) * a(m-k) * binom */
            sum += Ak * memo[m - k] * binom;

            if (k < m) {
                /* advance powers for next k */
                pow3 *= 3;
                pow9 *= 9;

                /* binom for k+1: C(m-1, k) = C(m-1, k-1) * (m-k) / k */
                binom = (binom * (seq_t)(m - k)) / (seq_t)k;
            }
        }
        memo[m] = sum;
    }

    memo_len = (size_t)n + 1;
    return memo[n];
}
