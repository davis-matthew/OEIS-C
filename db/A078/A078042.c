/*
 * A078042  Expansion of (1-x)/(1+x-x^2+x^3).
 * https://oeis.org/A078042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-2) - a(n-3), a(0)=1, a(1)=-2, a(2)=3
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 35, i64 n <= 71, i128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic cache for all values that have already been computed  */
static seq_t *cache          = NULL;           /* array of seq_t values          */
static idx_t cache_len        = 0;             /* number of valid entries        */
static idx_t cache_cap        = 0;             /* current allocation capacity    */

/* initialise cache with the first three terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 3;
    cache_len = 3;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)      /* allocation failure – give up */
        return;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-2;
    cache[2] = (seq_t)3;
}

/* enlarge the cache so that it can store indices up to n (inclusive)   */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL)
        init_cache();
    /* need capacity for indices 0..n inclusive */
    while (n >= cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)      /* realloc failed – leave as is */
            return;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute all terms up to and including n that have not yet been computed */
static void compute_until(idx_t n)
{
    /* start from the first index that is still uncomputed */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = -a(i-1) + a(i-2) - a(i-3) */
        cache[i] = -cache[i - 1] + cache[i - 2] - cache[i - 3];
    }
    cache_len = n + 1;  /* now first n+1 entries are valid */
}

/* public function that returns a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n >= cache_len)          /* value not yet computed */
        compute_until(n);
    return cache[(size_t)n];
}
