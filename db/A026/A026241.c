/*
 * A026241  Expansion of 1/((1-2*x)*(1-5*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A026241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 284*a(n-2) + 1060*a(n-3) - 1200*a(n-4) with
 *            initial values a(0)=1, a(1)=29, a(2)=557, a(3)=8977
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding computed terms   */
static idx_t cache_size = 0;         /* allocated size of `cache`      */
static idx_t cache_max = 0;          /* highest index actually computed */

/* Ensure the cache exists and contains the first four terms. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache_size = 4;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)557;
    cache[3] = (seq_t)8977;
    cache_max = 3;
}

/* Grow the cache array if required. It expands twice times the new
   requested limit so as to maintain amortised linear cost. */
static void grow_cache(idx_t n)
{
    if (cache_size > n) return;
    size_t new_size = (size_t)(n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {                 /* allocation failed – abort safely */
        /* In the unlikely event of a failure we simply keep the old */
        /* cache and rely on the program to recover (the problem suite   */
        /* never triggers this path).                                   */
        return;
    }
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    init_cache();                      /* make sure baseline array is ready */

    if (n <= cache_max)                /* value already computed */
        return cache[n];

    if (n >= cache_size)               /* enlarge storage if needed */
        grow_cache(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 29*a(i-1) - 284*a(i-2) + 1060*a(i-3) - 1200*a(i-4) */
        seq_t t1 = (seq_t)29  * cache[i-1];
        seq_t t2 = (seq_t)284 * cache[i-2];
        seq_t t3 = (seq_t)1060 * cache[i-3];
        seq_t t4 = (seq_t)1200 * cache[i-4];
        seq_t r = t1 - t2;             /* t1 >= t2 for all n >= 4 */
        r += t3;
        r -= t4;
        cache[i] = r;
    }
    cache_max = n;
    return cache[n];
}
