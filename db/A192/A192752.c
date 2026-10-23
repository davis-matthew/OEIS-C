/*
 * A192752  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3), a(0)=1, a(1)=7, a(2)=12
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;      /* sequence storage */
static idx_t  cache_len = 0;       /* number of elements currently computed */
static idx_t  cache_cap = 0;       /* allocated capacity of cache */

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cap(idx_t n)
{
    if (cache_cap > n) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n < 3) {
        static const seq_t base[3] = { (seq_t)1, (seq_t)7, (seq_t)12 };
        return base[n];
    }

    /* initialisation of cache if first call */
    if (cache_len == 0) {
        ensure_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)12;
        cache_len = 3;
    }

    if (n >= cache_len) {
        ensure_cap(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a[i] = 2*a[i-1] - a[i-3] */
            cache[i] = (cache[i-1] << 1) - cache[i-3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
