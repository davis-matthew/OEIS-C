/*
 * A201224  E.g.f.: -log(cos(x) - x).
 * https://oeis.org/A201224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the coefficient of x^n/n! in -log(cos(x)-x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the relation B(x)=exp(-A(x)), B(x)=cos(x)-x */
/* Uses double‑sum with binomial coefficients.  Works for all n≥1. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed. */
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* length of cache array (index of last stored a) */

    /* Grow the cache if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)n + 1; /* indices 0..n, we ignore cache[0] */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;

        /* Initialise new entries with zeros (uncomputed) */
        for (size_t i = (size_t)cache_len + 1; i < new_len; ++i)
            cache[i] = 0;

        /* Seed with first term if it hasn't been set yet */
        if (cache_len == 0) {
            cache[1] = 1;
            cache_len = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t m = cache_len; m < n; ++m) {
            /* we are computing a_{m+1} (so n_cur = m) */
            idx_t n_cur = m;

            /* Accumulate positive and negative contributions separately */
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;

            /* Binomial coefficient binom(n_cur, j).  Start with binom = 1 for j=0. */
            seq_t binom = 1;

            /* Loop over j = 1 .. n_cur.  Exclude j where b_j = 0 (odd > 1). */
            for (idx_t j = 1; j <= n_cur; ++j) {
                /* Update binomial coefficient to binom(n_cur, j) */
                binom = (binom * (n_cur - j + 1)) / j;

                /* Determine the sign of b_j */
                int sign = 0;          /* 0: ignore, 1: +1, -1: -1 */
                if (j == 1)
                    sign = -1;
                else if ((j & 1) == 0) { /* even j */
                    if (((j >> 1) & 1) == 0)
                        sign = 1;
                    else
                        sign = -1;
                } else {
                    /* odd j > 1 gives b_j = 0 */
                    sign = 0;
                }

                if (sign == 0) continue;

                /* index of the a value needed: a_{n_cur - j + 1} */
                idx_t a_index = n_cur - j + 1;
                seq_t a_val   = cache[a_index];

                seq_t prod = binom * a_val;

                if (sign > 0)
                    pos_sum += prod;
                else
                    neg_sum += prod;
            }

            /* Compute the coefficient b_{n_cur+1} (i.e., for n+1) */
            int sb = 0; /* sign of b_{n_cur+1} */
            idx_t idx_inv = n_cur + 1;
            if (idx_inv == 1)
                sb = -1;
            else if ((idx_inv & 1) == 0) {
                if (((idx_inv >> 1) & 1) == 0)
                    sb = 1;
                else
                    sb = -1;
            } else {
                sb = 0;
            }

            /* a_{m+1} = neg_sum - pos_sum - sb   (all unsigned computations) */
            seq_t res = neg_sum;
            if (sb == 1)
                res -= 1;
            else if (sb == -1)
                res += 1;
            res -= pos_sum;

            cache[m + 1] = res;
        }

        cache_len = n;
    }

    return cache[n];
}
