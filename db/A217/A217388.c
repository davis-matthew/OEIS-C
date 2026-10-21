/*
 * A217388  Alternating sums of the ordered Bell numbers (number of
 *          preferential arrangements) A000670.
 * https://oeis.org/A217388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} (-1)^(n-k) * t(k) where t(k) are the ordered
 *            Bell numbers (A000670). Using the identity t(k) = a(k) + a(k-1)
 *            and t(0)=1, a(0)=1 we obtain the simple recurrence a(0) = 1 a(n)
 *            = t(n) - a(n-1) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for t(n) (ordered Bell numbers)
   t(0) = 1
   t(n) = sum_{i=0}^{n-1} binom(n,i) * t(i)   (n ≥ 1)                 */
/* The implementation precomputes and memoises the values indexed
   from 0 up to the largest n requested.                          */

#include <stdlib.h>

static seq_t *t_cache = NULL;   /* cache of ordered Bell numbers */
static seq_t *a_cache = NULL;   /* cache of a(n) values          */
static idx_t max_computed = 0;  /* highest index that has been computed */
static size_t cache_cap   = 0;   /* size of allocated cache        */

/* ensure that cache arrays hold at least capacity entries */
static void ensure_cache_capacity(idx_t capacity)
{
    if (cache_cap >= capacity)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < (size_t)capacity)
        new_cap *= 2;
    t_cache = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* compute a(n) (sequence A217388) */
seq_t a(idx_t n)
{
    /* initialise caches if first call */
    if (t_cache == NULL) {
        ensure_cache_capacity(1);
        t_cache[0] = (seq_t)1;
        a_cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* return cached value if already computed */
    if (n <= max_computed)
        return a_cache[n];

    /* compute up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* ensure capacity for new index */
        ensure_cache_capacity(k + 1);

        /* compute t(k) using recurrence */
        seq_t sum_t = (seq_t)0;
        seq_t binom = (seq_t)1;           /* binom(k,0) */
        for (idx_t i = 0; i < k; ++i) {
            sum_t += binom * t_cache[i];
            /* update binom(k,i+1) = binom(k,i) * (k-i) / (i+1) */
            binom = (binom * (seq_t)(k - i)) / (seq_t)(i + 1);
        }
        t_cache[k] = sum_t;

        /* a(k) = t(k) - a(k-1) (with a(-1)=0 for k=0 already handled) */
        a_cache[k] = t_cache[k] - a_cache[k - 1];
    }

    max_computed = n;
    return a_cache[n];
}
