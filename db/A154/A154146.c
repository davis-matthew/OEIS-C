/*
 * A154146  Numbers k such that 16 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A154146: numbers k such that 16 + k*(k+1)/2 is a perfect square.
    Recurrence (valid for n ≥ 5):
    a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5),
    with initial values a(0)=0, a(1)=14, a(2)=17, a(3)=87, a(4)=104. */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cache of computed values       */
static idx_t  cache_len  = 0;      /* number of values stored        */
static size_t cache_cap  = 0;      /* allocated capacity (in seq_t)  */

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;   /* already computed */

    /* grow the cache to accommodate index n */
    size_t new_cap = cache_cap ? cache_cap : 1;
    if (new_cap < 5)     /* at least room for the first five terms */
        new_cap = 5;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation must succeed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first five terms on first allocation */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)87;
        cache[4] = (seq_t)104;
        cache_len = 5;
    }

    /* compute missing terms using the recurrence */
    if (n >= cache_len) {
        idx_t start = cache_len < 5 ? 5 : cache_len;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (cache[i-2] * (seq_t)6)
                     - (cache[i-3] * (seq_t)6)
                     - cache[i-4]
                     + cache[i-5];
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
