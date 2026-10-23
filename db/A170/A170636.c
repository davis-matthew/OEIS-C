/*
 * A170636  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; for 1 ≤ n < 48, a(n) = 3·2^(n-1);
 * for n ≥ 48 the sequence satisfies the linear recurrence
 *   a(n) = 2a(n‑1) – a(n‑2) + 2a(n‑3) – a(n‑4) + … + 2a(n‑47) – a(n‑48),
 * i.e. coefficients alternate 2, –1, 2, –1, … for the previous 48 terms.
 */

#include <stdlib.h>

/* Cached terms */
static seq_t *cache = NULL;
static idx_t cached_size = 0;   /* number of terms already computed (next index to fill) */
static size_t cache_cap = 0;    /* capacity of the cache */

/* Ensure the cache can store terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? n + 1 : cache_cap * 2;
    if (new_cap < n + 1)
        new_cap = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
    {
        /* In a contest environment we could exit, but here we simply abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute terms up to n */
    while (cached_size <= n)
    {
        idx_t i = cached_size;

        if (i == 0)
        {
            cache[i] = (seq_t)1;
        }
        else if (i < 48)          /* 1 ≤ i < 48: closed‑form values */
        {
            if (i == 1)
                cache[i] = (seq_t)3;          /* 3·2^(0) */
            else
                cache[i] = cache[i - 1] * (seq_t)2;  /* multiply previous by 2 */
        }
        else                       /* i ≥ 48: linear recurrence */
        {
            seq_t pos = 0;  /* sum of terms with coefficient +2 */
            seq_t neg = 0;  /* sum of terms with coefficient –1 */

            for (idx_t j = 1; j <= 48; ++j)
            {
                seq_t prev = cache[i - j];
                if (j & 1)          /* odd j → coefficient +2 */
                {
                    pos += prev * (seq_t)2;
                }
                else                /* even j → coefficient –1 */
                {
                    neg += prev;
                }
            }
            cache[i] = pos - neg;
        }

        ++cached_size;
    }

    return cache[n];
}
