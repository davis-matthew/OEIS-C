/*
 * A170528  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170528 derived from its rational generating function
   (see OEIS entry).  The sequence satisfies:
   a(0) = 1
   For 1 ≤ n ≤ 45    : a(n) = 2 + 37 · ∑_{i=0}^{n‑1} a(i)
   For n = 46             : a(46) = 1 + 37 · ∑_{i=1}^{45} a(i) – 703 · a(0)
   For n > 46             : a(n) = 37 · ∑_{i=n-45}^{n-1} a(i) – 703 · a(n-46)
   The function memoises results so repeated calls are O(1).  The logic
   works for any seq_t type (built‑in or arbitrary‑precision). */

#include <stdlib.h>

/* Helper types are supplied by the build system (seq_t, idx_t). */

/* Memoisation cache */
static seq_t *cache = NULL;        /* array of a(i) values */
static idx_t  cache_len = 0;       /* number of values stored so far */
static idx_t  cache_cap = 0;       /* current capacity of cache array */

/* Cumulative sums needed for the recurrence */
static seq_t  sum_total = 0;       /* Σ_{i=0}^{k} a(i) up to current k */
static seq_t  sum_last45 = 0;      /* Σ_{i=k-45}^{k-1} a(i) used for n>46 */

/* Ensure the cache can hold index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = (cache_cap == 0 ? 1 : cache_cap);
    while (new_cap <= n)
        new_cap <<= 1;
    /* Cast is required for C++ when seq_t is a class type. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                /* abort on allocation failure */
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)               /* already computed */
        return cache[n];

    /* Grow cache up to n */
    ensure_capacity(n);

    const seq_t C37 = (seq_t)37;
    const seq_t C703 = (seq_t)703;

    for (idx_t k = cache_len; k <= n; ++k)
    {
        seq_t val = 0;

        if (k == 0)                 /* base case */
        {
            val = (seq_t)1;
            sum_total = val;        /* first cumulative sum */
        }
        else if (k <= 45)           /* use running total of all previous terms */
        {
            val = (seq_t)2 + C37 * sum_total;
            sum_total += val;
        }
        else if (k == 46)           /* special middle term */
        {
            seq_t sum_1to45 = sum_total - cache[0];
            val = (seq_t)1 + C37 * sum_1to45 - C703 * cache[0];
            sum_total += val;
            sum_last45 = sum_1to45; /* initialise window for future steps */
        }
        else                        /* k > 46  : sliding‑window recurrence */
        {
            /* update window first, then compute value */
            sum_last45 = sum_last45 - cache[k - 46] + cache[k - 1];
            val = C37 * sum_last45 - C703 * cache[k - 46];
            sum_total += val;
        }

        cache[k] = val;              /* store computed term */
    }

    cache_len = n + 1;               /* cache now valid up to n */
    return cache[n];
}
