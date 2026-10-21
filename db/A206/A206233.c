/*
 * A206233  Number of (n+1) X 4 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 129*a(n-3) - 129*a(n-4) for n > 7 Initial terms:
 *            a(1)=310, a(2)=1136, a(3)=4456, a(4)=27168, a(5)=133392,
 *            a(6)=607008, a(7)=3503136
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t a_init[7] = {
    (seq_t)310,  (seq_t)1136,  (seq_t)4456,
    (seq_t)27168, (seq_t)133392, (seq_t)607008,
    (seq_t)3503136
};

static seq_t *cache      = NULL;   /* array of computed a(n), 0‑based index */
static size_t  cached_len = 0;     /* how many entries have been computed */
static size_t  cache_cap  = 0;     /* allocated capacity */

/* Allocate/extend the cache to hold at least 'need' values (1‑based). */
static void ensure_cache(idx_t need)
{
    if ((size_t)need <= cached_len)      /* already computed up to need */
        return;

    /* grow capacity if necessary */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries from cached_len+1 up to need */
    for (size_t i = cached_len + 1; i <= (size_t)need; ++i) {
        size_t idx0 = i - 1; /* 0‑based index for a(i) */
        if (i <= 7) {                /* initial values */
            cache[idx0] = a_init[i - 1];
        } else {                     /* recurrence: a(n) = a(n-1)+129*a(n-3)-129*a(n-4) */
            seq_t prev = cache[idx0 - 1];              /* a(n-1) */
            seq_t diff = cache[idx0 - 3] - cache[idx0 - 4]; /* a(n-3) - a(n-4) */
            cache[idx0] = prev + diff * (seq_t)129;   /* a(n-1) + 129*diff */
        }
    }
    cached_len = need;
}

seq_t a(idx_t n)
{
    if (n < 1)     /* undefined for offset 1, return 0 */
        return (seq_t)0;

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
