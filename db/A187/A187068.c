/*
 * A187068  Let i be in {1,2,3}, let r >= 0 be an integer and n=2*r+i-1. Then
 *          a(n)=a(2*r+i-1) gives the quantity of H_(7,1,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x=sqrt((2*cos(Pi/7))^2-1).
 * https://oeis.org/A187068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 60, u64 n <= 114,
 *            u128 n <= 224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = 2*a(n-2) + a(n-4) - a(n-6)   (n ≥ 6)
 * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *                 a(4)=1, a(5)=1
 */
#include <stdlib.h>

static seq_t* cache = NULL;        /* array of computed terms */
static size_t  cache_cap = 0;      /* current capacity of cache    */
static idx_t  computed_max = 0;    /* next index that needs computing */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    if ((size_t)need < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize base values (if not yet done). */
static void init_base(void)
{
    if (cache != NULL)
        return;
    /* Allocate at least 6 entries. */
    cache_cap = 6;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)0;
    cache[4] = (seq_t)1;
    cache[5] = (seq_t)1;
    computed_max = 6;           /* next term that requires computation */
}

seq_t a(idx_t n)
{
    init_base();                     /* ensure cache and base terms exist */
    if ((idx_t)n < 6)                /* return a pre‑computed base value */
        return cache[(size_t)n];

    /* Ensure we have enough space in the cache. */
    ensure_cache(n);
    /* Compute missing terms up to n. */
    for (idx_t i = computed_max; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-2) + a(i-4) - a(i-6) */
        seq_t t1 = cache[(size_t)(i - 2)];
        seq_t t2 = cache[(size_t)(i - 4)];
        seq_t t3 = cache[(size_t)(i - 6)];
        cache[(size_t)i] = (seq_t)( (seq_t)2 * t1 + t2 - t3 );
    }
    computed_max = n + 1;            /* update state */
    return cache[(size_t)n];
}
