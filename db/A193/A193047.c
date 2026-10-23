/*
 * A193047  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            6*a(n-1)-14*a(n-2)+15*a(n-3)-5*a(n-4)-4*a(n-5)+4*a(n-6)-a(n-7)
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=19, a(4)=102, a(5)=377, a(6)=1104
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoisation array */
static idx_t  cache_size = 0;      /* current allocated capacity */
static idx_t  cache_max  = (idx_t)-1; /* highest index with valid value */

/* initialise the cache with a small default size */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialised */

    cache_size = 16;                    /* start with room for 16 entries */
#ifdef __cplusplus
    cache = new (std::nothrow) seq_t[cache_size];
    if (!cache) std::abort();
#else
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache) exit(1);
#endif
}

/* grow the cache to hold indices 0..n at least */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;         /* already large enough */

    idx_t new_size = cache_size;
    while (new_size <= n) new_size <<= 1;   /* exponential growth */

#ifdef __cplusplus
    seq_t *tmp = new (std::nothrow) seq_t[new_size];
    if (!tmp) std::abort();
    for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
#endif
    cache_size = new_size;
}

/* compute a[i] for all i up to and including n, storing them in cache */
static void compute_upto(idx_t n)
{
    init_cache();
    ensure_cache(n);

    /* first initialise the base terms if this is the first call */
    if (cache_max == (idx_t)-1) {
        const seq_t base[7] = {0,1,2,19,102,377,1104};
        for (idx_t i = 0; i < 7; ++i) cache[i] = base[i];
        cache_max = 6;
    }

    if (n <= cache_max) return;        /* nothing to do */

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* compute using a safe two‑step subtraction: sum_pos - sum_neg   */
        seq_t sum_pos =  6 * cache[i-1]  /* 6*a(n-1) */
                      + 15 * cache[i-3] /* +15*a(n-3) */
                      + 4  * cache[i-6];/* +4*a(n-6) */
        seq_t sum_neg = 14 * cache[i-2] /* -14*a(n-2) */
                      +5  * cache[i-4] /* -5*a(n-4) */
                      +4  * cache[i-5] /* -4*a(n-5) */
                      +  cache[i-7];   /* -a(n-7) */

        cache[i] = sum_pos - sum_neg;
    }
    cache_max = n;
}

/* exported sequence function */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[n];
}
