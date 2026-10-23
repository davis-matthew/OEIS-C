/*
 * A169358  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -28*a(n-31) + 7*∑_{k=1}^{30} a(n-k) (n ≥ 31) * Initial
 *            values a(0)…a(30) are generated from the generating function *
 *            ((t^31 + 2t^30 + … + 2t + 1) / (1 - 7t - … - 7t^30 + 28t^31)). *
 *            The first 20 terms are: 1, 9, 72, 576, 4608, 36864, 294912,
 *            2359296, * 18874368, 150994944, 1207959552, 966...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of already-computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* generate the first 31 values a(0)…a(30) */
static void init_seeds(void)
{
    if (cache != NULL)
        return;

    idx_t const N = 31;                    /* number of seeds needed   */
    cache = (seq_t *)malloc(N * sizeof(seq_t));
    if (!cache) return;                    /* memory allocation failure */

    /* a[0] = 1  ;  for k = 1..30  :  a[k] = 2 + 7 * sum_{i=0}^{k-1} a[i]   */
    cache[0] = (seq_t)1;
    seq_t pref = cache[0];                 /* prefix sum of already known terms */

    for (idx_t k = 1; k < N; ++k) {
        /* numerator coefficient for k (k=1..30) is 2 */
        cache[k] = (seq_t)2 + (seq_t)7 * pref;
        pref += cache[k];
    }

    cache_size = N;
}

/* ensure that the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;                      /* memory allocation failure */
    cache = tmp;
    cache_size = new_size;
}

/* main routine to return a(n) */
seq_t a(idx_t n)
{
    /* make sure seeds are available */
    init_seeds();

    if (n < cache_size)
        return cache[n];

    /* grow cache to the requested size */
    ensure_cache(n);

    /* compute values from cache_size (31) up to n */
    /* sliding window of the last 30 terms is needed for the recurrence */
    seq_t sum_window = 0;
    for (idx_t j = 1; j <= 30; ++j)
        sum_window += cache[j];            /* sum of a[1] … a[30] */

    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 7*sum_window - 28*a(i-31) */
        seq_t val = (seq_t)7 * sum_window;
        val -= (seq_t)28 * cache[i - 31];
        cache[i] = val;

        /* update the sliding window for the next iteration */
        sum_window += cache[i];            /* add the new term   */
        sum_window -= cache[i - 30];       /* remove the oldest term */
    }

    return cache[n];
}
