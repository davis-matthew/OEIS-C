/*
 * A169515  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n_n + 20·a(n‑1) + 20·a(n‑2) + … + 20·a(n‑33) –
 *            210·a(n‑34) where n_n = 1 for n = 0 or 34, 2 for 1 ≤ n ≤ 33, and
 *            0 for n > 34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* stores already computed terms */
static size_t cache_cap = 0;   /* number of allocated slots in cache     */
static idx_t  max_computed = (idx_t)-1;  /* last index that has been computed */

/* compute a(n) using memoized linear recurrence */
seq_t a(idx_t n)
{
    /* grow cache if necessary */
    if (cache == NULL || (size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise missing terms */
    idx_t start = (max_computed == (idx_t)-1) ? 0 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        /* value coming from the numerator of the generating function */
        seq_t n_val;
        if (i == 0 || i == 34)
            n_val = (seq_t)1;
        else if (i <= 34)
            n_val = (seq_t)2;
        else
            n_val = (seq_t)0;

        /* sum of the previous 33 terms (if they exist) */
        seq_t sum_prev = (seq_t)0;
        for (idx_t j = 1; j <= 33 && j <= i; ++j)
            sum_prev += cache[i - j];

        /* compute a(i) from the recurrence */
        seq_t term = n_val + sum_prev * (seq_t)20;
        if (i >= 34)
            term -= cache[i - 34] * (seq_t)210;

        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
