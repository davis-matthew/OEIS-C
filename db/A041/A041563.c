/*
 * A041563  Denominators of continued fraction convergents to sqrt(299).
 * https://oeis.org/A041563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 830*a(n-4) - a(n-8) for n>7 Initial values: a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=24, a(4)=823, a(5)=2493, a(6)=5809,
 *            a(7)=19920
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* memoized values of the sequence   */
static idx_t  cache_len = 0;     /* number of elements currently stored (cache_len-1 is max index) */

/* Ensure that cache holds values up to index n.  When the cache is first
   initialized it is filled with the eight known initial terms.  For
   subsequent calls the sequence is extended only as far as needed. */
static void
ensure_cache(idx_t n)
{
    /* Nothing to do if we already have the requested term */
    if (cache != NULL && cache_len > n)
        return;

    /* First allocation: reserve space for at least 8 values and fill them. */
    if (cache == NULL) {
        idx_t init_len = 8;
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * init_len);
        if (!cache) abort();

        cache[0] = 1;      cache[1] = 3;      cache[2] = 7;      cache[3] = 24;
        cache[4] = 823;    cache[5] = 2493;   cache[6] = 5809;   cache[7] = 19920;
        cache_len = init_len;
    }

    /* Grow the cache if it does not yet contain the requested index. */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* Recurrence: a(i) = 830*a(i-4) - a(i-8) */
            cache[i] = (seq_t)830 * cache[i - 4] - cache[i - 8];
        }
        cache_len = new_len;
    }
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
