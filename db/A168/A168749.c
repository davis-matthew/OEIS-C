/*
 * A168749  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, for 1≤n≤17 : a(n)=2+22·(a(0)+…+a(n-1));
 *            a(18)=1+22·(a(1)+…+a(17))−253·a(0); for n≥19 :
 *            a(n)=22·(a(n-1)+…+a(n-17))−253·a(n-18);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache of computed terms      */
static size_t cache_cap = 0;      /* allocated size  */
static size_t cache_len = 0;      /* number of terms stored      */

/* resize cache so that it can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap ?: 1;
        while (new_cap < needed)
            new_cap <<= 1;                     /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute and store initial terms 0…18 (14 if n<18) */
static void init_terms(void)
{
    seq_t cumulative;          /* sum of all computed terms so far */
    if (cache_len >= 19) return;   /* already initialized */

    /* ensure we have room for indices 0…18 */
    ensure_capacity(18);

    /* a(0)=1 */
    cache[0] = 1;
    cumulative = 1;
    cache_len = 1;

    /* a(n)=2+22·cumulative for 1≤n≤17 */
    while (cache_len <= 17)
    {
        seq_t val = 2 + 22 * cumulative;
        cache[cache_len] = val;
        cumulative += val;
        cache_len++;
    }

    /* a(18)=1+22·sum_{i=1}^{17}a(i)−253·a(0) */
    seq_t sum17 = 0;
    for (int j = 1; j <= 17; ++j)
        sum17 += cache[18 - j];                /* a(17)…a(1) */

    cache[18] = 1 + 22 * sum17 - 253 * cache[0];
    cache_len = 19;                            /* indices 0…18 stored */
}

seq_t a(idx_t n)
{
    /* initialise cache and compute up to needed index */
    if (!cache) init_terms();          /* ensures a(0)…a(18) computed */

    if ((idx_t)cache_len <= n)
        ensure_capacity(n);

    /* compute terms beyond the currently stored ones */
    while ((idx_t)cache_len <= n)
    {
        const size_t i = cache_len;    /* index to compute */

        /* recurrence for n≥19 : a(n)=22·(a(n-1)+…+a(n-17))−253·a(n-18) */
        seq_t sum_17 = 0;
        for (int j = 1; j <= 17; ++j)
            sum_17 += cache[i - j];
        seq_t val = 22 * sum_17 - 253 * cache[i - 18];
        cache[i] = val;
        cache_len++;
    }

    return cache[n];
}
