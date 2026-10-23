/*
 * A170439  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p[n] + 44 * Σ_{k=1}^{min(n,43)} a[n‑k] – (n≥44 ? 990 *
 *            a[n‑44] : 0) where p[0] = 1, p[44] = 1, and p[n] = 2 for 1 ≤ n ≤
 *            43. This recurrence follows from the rational generating
 *            function G(t) = (t^44 + 2 t^43 + … + 2 t + 1) / (1 – 44 t – 44
 *            t^2 – … – 44 t^43 + 990 t^44)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------------- */
static seq_t *seq_cache = NULL;     /* cache of a(i) values                */
static idx_t  cache_last = 0;       /* largest index for which a(k) is cached (≥0) */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (seq_cache == NULL) { /* first call */
        seq_cache = (seq_t *)malloc((size_t)(1) * sizeof(seq_t));
        if (seq_cache == NULL) exit(1);  /* out of memory */
        seq_cache[0] = (seq_t)1;         /* a(0) = 1 */
        cache_last = 0;
    }
    if ((size_t)(cache_last + 1) < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(1);        /* out of memory */
        seq_cache = tmp;
        /* Values beyond cache_last are not initialized yet; they will be set in the loop below. */
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i <= 43 ? i : 43);   /* number of terms to sum over */
        for (idx_t j = 1; j <= limit; ++j)
            sum += seq_cache[i - j];

        seq_t term = (i >= 44) ? (seq_t)990 * seq_cache[i - 44] : (seq_t)0;

        seq_t p_val;
        if (i == 0)
            p_val = (seq_t)1;
        else if (i == 44)
            p_val = (seq_t)1;
        else
            p_val = (seq_t)2;

        seq_cache[i] = p_val + (seq_t)44 * sum - term;
    }
    cache_last = n;
    return seq_cache[n];
}
