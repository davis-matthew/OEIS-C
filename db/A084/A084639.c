/*
 * A084639  Expansion of x*(1+2*x)/((1+x)*(1-x)*(1-2*x)).
 * https://oeis.org/A084639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3, with a(0)=0 and a(1)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of already computed terms */
static seq_t *term_cache = NULL;
static idx_t cache_len   = 0;    /* number of terms currently stored (cache_len > 0 -> cache_len >= 2) */
static idx_t cache_cap   = 0;    /* allocated capacity */

/* extend the cache to at least `new_cap` elements */
static void ensure_cache_cap(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* grow at least twice */
    if (cache_cap < 4)
        cache_cap = 4;
    while (cache_cap < new_cap)
        cache_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
}

/* initialize the cache with base values 0 and 1 */
static void init_cache(void)
{
    if (cache_len == 0)
    {
        ensure_cache_cap(2);
        cache_len = 2;
        term_cache[0] = (seq_t)0;
        term_cache[1] = (seq_t)1;
    }
}

/* compute a(n) using recursion with memoization */
seq_t a(idx_t n)
{
    init_cache();                     /* guarantee base terms present */

    if (n < cache_len)
        return term_cache[n];

    /* grow cache to store up to index n */
    ensure_cache_cap(n + 1);

    /* compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
            val = (seq_t)0;          /* already stored, but keep for clarity */
        else if (i == 1)
            val = (seq_t)1;          /* already stored */
        else
        {
            /* a(i) = a(i-1) + 2*a(i-2) + 3 */
            seq_t two_a2 = (seq_t)( (seq_t)2 * term_cache[i-2] );
            val = term_cache[i-1] + two_a2 + (seq_t)3;
        }
        term_cache[i] = val;
    }
    cache_len = n + 1;
    return term_cache[n];
}
