/*
 * A103365  First column of triangle A103364, which equals the matrix inverse
 *          of the Narayana triangle (A001263).
 * https://oeis.org/A103365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Σ_{k=0}^{n-1} a(k) * C(n,k) * C(n+1,k+1) / (n-k+1)
 *            (with offset 1; a(1)=1). The recurrence is applied with internal
 *            index i = n-1 and a(0)=1 (corresponding to a(1))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 2,
 *            big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdalign.h>
#include "../utils/mul128.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)  /* unused offset handling */
        return 0;

    idx_t idx = n - 1;                 /* internal index (0‑based) */

    /* Grow cache if needed */
    if (idx + 1 > cache_size)
    {
        idx_t new_size = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)               /* allocation failure – abort */
            return 0;
        cache = new_cache;
    }

    /* Compute values from cache_size up to idx inclusive */
    for (idx_t i = cache_size; i <= idx; ++i)
    {
        if (i == 0)                  /* base value: a(0) = 1 (internal) */
        {
            cache[0] = 1;
        }
        else
        {
            seq_t sum = 0;
            seq_t binom = 1;          /* C(i,0) */

            for (idx_t k = 0; k < i; ++k)
            {
                /* Update binom = C(i,k) for k>0 */
                if (k > 0)
                {
                    seq_t num = mul128(binom, (seq_t)(i - k + 1));
                    binom = num / (seq_t)k;
                }

                /* binom_next = C(i+1, k+1) */
                seq_t binom_next = mul128(binom, (seq_t)(i + 1)) / (seq_t)(k + 1);

                /* term = a(k) * binom * binom_next / (i - k + 1) */
                seq_t term = cache[k];
                term = mul128(term, binom);
                term = mul128(term, binom_next);
                term = term / (seq_t)(i - k + 1);

                sum += term;
            }
            cache[i] = -sum;
        }
        cache_size = i + 1;
    }

    return cache[idx];
}
