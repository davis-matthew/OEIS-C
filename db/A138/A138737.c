/*
 * A138737  The n-th term of the n-th inverse binomial transform of this
 *          sequence equals (n+1)^(n-1) for n>=0.
 * https://oeis.org/A138737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence a(n) satisfies * a(0) = 1 * for n ≥ 1, * a(n) =
 *            (n+1)^(n-1) + Σ_{k=1}^{n} (-1)^k · n^k · binom(n,k) · a(n-k) *
 *            (i.e. Σ_{k=0}^n (-1)^k · n^k · binom(n,k) · a(n-k) =
 *            (n+1)^(n-1)) * This follows from the ordinary generating
 *            function condition of A138737
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size of callback */
    static idx_t computed_n = 0;   /* highest n whose a(n) is stored */

    /* first time: create cache[0] = 1 */
    if (cache == NULL) {
        cache = seq_realloc(NULL, 0, 1);
        cache[0] = 1;        /* a(0) = 1 */
        cache_size = 1;
        computed_n = 0;
    }

    if (n <= computed_n)            /* already computed */
        return cache[n];

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        cache = seq_realloc(cache, cache_size, n + 1);
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 0;
        cache_size = n + 1;
    }

    /* compute new values */
    for (idx_t i = computed_n + 1; i <= n; ++i) {
        /* rhs = (i+1)^(i-1) (big number) */
        seq_t rhs = big_pow((seq_t)(i + 1), (seq_t)(i >= 1 ? i - 1 : 0));

        /* accumulate sum = Σ_{k=1}^i (-1)^k·n^k·binom(n,k)·a(i-k) */
        seq_t sum_pos = 0, sum_neg = 0;          /* positive and negative parts separately */
        seq_t binom = 1;                         /* binom(i,0) */
        seq_t pow_nk = 1;                        /* i^0 */
        int  sign = -1;                          /* (-1)^1 for first k */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial: binom = binom * (i-k+1) / k */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            /* update power: pow_nk = pow_nk * i */
            pow_nk = pow_nk * (seq_t)i;

            /* coefficient = binom * pow_nk  (positive magnitude) */
            seq_t coeff = binom * pow_nk;

            /* term = coeff * a(i-k)  (unsigned value) */
            seq_t term = cache[i - k] * coeff;

            if (sign == 1)
                sum_pos += term;
            else
                sum_neg += term;

            sign = -sign;   /* flip sign for next k */
        }

        /* a(i) = rhs + sum_neg - sum_pos */
        seq_t tmp = rhs + sum_neg;
        seq_t ai = tmp - sum_pos;
        cache[i] = ai;
    }

    computed_n = n;
    return cache[n];
}
