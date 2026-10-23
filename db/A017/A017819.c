/*
 * A017819  Expansion of 1/(1-x^3-x^4-x^5-x^6).
 * https://oeis.org/A017819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6) Initial values: a(0)=1,
 *            a(1)=a(2)=0, a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 21, u16 n <= 38, u32 n <= 73, u64 n <= 142,
 *            u128 n <= 279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;   /* cache of computed terms          */
static idx_t  cache_len = 0;  /* current allocated size of cache  */
static idx_t  max_computed = 0; /* highest index that has been computed */

/* enlarge the cache so that it can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;           /* already big enough */

    idx_t new_len = cache_len ? cache_len : 128;
    while (new_len <= n) new_len <<= 1;  /* double until huge enough */

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                   /* out of memory */

    /* initialise new elements to zero */
    for (idx_t i = cache_len; i < new_len; ++i) tmp[i] = 0;

    cache = tmp;
    cache_len = new_len;
}

/* initialise the first six values if they have not been set yet */
static void init_base(void)
{
    if (max_computed > 0) return;        /* already initialised */

    ensure_cache(5);                     /* need indices 0 … 5 */

    cache[0] = 1;       /* a(0) */
    cache[1] = 0;       /* a(1) */
    cache[2] = 0;       /* a(2) */
    cache[3] = 1;       /* a(3) */
    cache[4] = 1;       /* a(4) */
    cache[5] = 1;       /* a(5) */

    max_computed = 5;
}

/* compute all terms up to n (if not already computed) */
static void compute_up_to(idx_t n)
{
    init_base();                 /* guarantee that a(0)…a(5) exist */

    if (n <= max_computed) return;   /* nothing to do */

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-3)+a(i-4)+a(i-5)+a(i-6) */
        cache[i] = cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
    }
    max_computed = n;
}

/* ------------------------------------------------------------------ */
/* Public function required by the specification                     */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
