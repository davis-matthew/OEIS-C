/*
 * A192857  Number of matchings in the n-web graph.
 * https://oeis.org/A192857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) + a(n-3) - a(n-4) (n >= 4), with
 *            initial values a(0)=4, a(1)=3, a(2)=23, a(3)=93
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms stored (valid indices 0 .. cache_size-1) */
static idx_t cache_cap = 0;    /* allocated capacity */

/* Grow the cache to accommodate at least 'need' elements. */
static void cache_ensure(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* If memory allocation fails, abort. The sequence is not required
           to be computed in that case. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize the cache with the first four terms if it has not been done
   yet. */
static void cache_init(void)
{
    if (cache_size == 0) {
        cache_ensure(4);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)93;
        cache_size = 4;
    }
}

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    cache_init();
    if (n >= cache_size) {
        cache_ensure(n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            cache[i] = (seq_t)3 * t1 + (seq_t)7 * t2 + t3 - t4;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
