/*
 * A176609  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=5, k=0 and l=1.
 * https://oeis.org/A176609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 5, a(n+1) = sum_{p=0..n} a(p)*a(n-p) + 1 (for n
 *            >= 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cached values of the sequence */
static idx_t cache_size = 0;   /* number of values currently stored (>= 2 after init) */
static idx_t cache_cap  = 0;   /* allocated capacity in the cache array */

/* Ensure that the cache contains values a(0) ... a(n) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialize cache for a(0) and a(1) */
        idx_t init_cap = 2;
        cache = (seq_t*)malloc((size_t)init_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_size = 2;
        cache_cap  = init_cap;
    }

    if (n < cache_size)
        return;

    /* grow the cache to at least n+1 items (power‑of‑two growth for simplicity) */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_buf = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* compute missing values lazily */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* a(k) is defined only for k >= 2 by the recurrence            */
        /* a(0) and a(1) are already set; therefore the loop starts at 2 */
        if (k < 2) {
            cache[k] = (k == 0) ? (seq_t)1 : (seq_t)5;
            continue;
        }

        seq_t sum = 0;
        /* sum_{p=0..k-1} a(p) * a(k-1-p)  + 1  */
        for (idx_t p = 0; p < k; ++p)
            sum += cache[p] * cache[k - 1 - p];

        cache[k] = sum + (seq_t)1;
    }

    cache_size = n + 1;
}

/* External function required by the framework
   Computes a(n) according to the described recurrence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
