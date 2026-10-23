/*
 * A041805  Denominators of continued fraction convergents to sqrt(423).
 * https://oeis.org/A041805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 9214*a(n-8) - a(n-16) (n > 15) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=7, a(4)=30, a(5)=97, a(6)=127, a(7)=224,
 *            a(8)=9087, a(9)=9311, a(10)=18398, a(11)=64505, a(12)=276418,
 *            a(13)=893759, a(14)=1170177, a(15)=2063936
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_tbl[16] = {
    1, 1, 2, 7, 30, 97, 127, 224,
    9087, 9311, 18398, 64505, 276418, 893759,
    1170177, 2063936
};

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t cache_len = 0;          /* number of terms stored */

/* expand cache to at least 'need' terms (indices 0 .. need-1) */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;  /* allocation failure: caller will handle via NULL check */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) using recurrence; cache results for future calls */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;      /* n is uint64_t, fits into size_t on 64‑bit arch */

    /* expand cache to include term idx */
    ensure_cache(idx + 1);
    if (cache_len > idx)
        return cache[idx];

    /* compute missing terms */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val;
        if (i < 16) {
            val = init_tbl[i];
        } else {
            val = (seq_t)9214 * cache[i - 8] - cache[i - 16];
        }
        if (i < cache_cap)
            cache[i] = val;
    }
    cache_len = idx + 1;
    return cache[idx];
}
