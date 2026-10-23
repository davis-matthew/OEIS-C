/*
 * A128232  Expansion of exp(x)/(1 - x^4/4!), where a(n) = 1 + C(n,4)*a(n-4).
 * https://oeis.org/A128232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=1, a(n) = 1 + C(n,4) * a(n-4) for n ≥ 4
 *            where C(n,4) = n·(n-1)·(n-2)·(n-3)/24 (recurrence matches
 *            exp(x)/(1 - x^4/4!) )
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoised values */
static seq_t *cache       = NULL;
static idx_t   cache_cap  = 0;   /* allocated size */
static idx_t   computed_max = 0; /* largest index computed */

/* compute binomial coefficient C(n,4) without intermediate overflows */
static seq_t binom4(idx_t n)
{
    seq_t res = (seq_t)n;
    res = res * (seq_t)(n - 1) / 2; /* divide by 2 first */
    res = res * (seq_t)(n - 2) / 3; /* divide by 3 next */
    res = res * (seq_t)(n - 3) / 4; /* divide by 4 last   */
    return res;                     /* result = n(n‑1)(n‑2)(n‑3)/24 */
}

/* grow cache array so that [0 … need] are valid indices */
static void ensure_capacity(idx_t need)
{
    while (cache_cap <= need) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* initialise cache on first call                              */
    if (!cache) {
        ensure_capacity(3);                 /* store indices 0..3 */
        cache[0] = cache[1] = cache[2] = cache[3] = 1;
        computed_max = 3;
    }

    /* make sure the array covers the requested index            */
    ensure_capacity(n);

    /* return already computed value if available                */
    if (n <= computed_max) return cache[n];

    /* compute missing values in ascending order                */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val;
        if (i < 4)        /* never reached, but keeps compiler happy */
            val = 1;
        else
            val = (seq_t)1 + binom4(i) * cache[i - 4];
        cache[i] = val;
    }
    computed_max = n;
    return cache[n];
}
