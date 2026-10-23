/*
 * A024958  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with a(1)=3
 *          and a(2)=a(3)=1.
 * https://oeis.org/A024958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), a(1)=3,
 *            a(2)=a(3)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest n for which a(n) is computed */
static idx_t cache_cap = 0;    /* allocated size of cache */

static void ensure_cache(idx_t n)
{
    /* Initialise base cases if not already done */
    if (cache_size < 3) {
        const idx_t need_init = 3;
        if (cache_cap < need_init) {
            cache_cap = need_init * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)3; /* a(1) */
        cache[1] = (seq_t)1; /* a(2) */
        cache[2] = (seq_t)1; /* a(3) */
        cache_size = 3;
    }

    if (n <= cache_size)
        return;

    /* Grow cache if necessary */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n)
            new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms incrementally */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t sum = 0;
        idx_t limit = (k + 2) / 4;   /* floor((k+2)/4) */
        for (idx_t i = 1; i <= limit; ++i) {
            idx_t left_idx  = 2 * i - 1;          /* 1‑based */
            idx_t right_idx = k - 2 * i + 1;      /* 1‑based */
            seq_t left  = cache[left_idx - 1];
            seq_t right = cache[right_idx - 1];
            sum += left * right;
        }
        cache[k - 1] = sum;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1; return 0 as placeholder */

    ensure_cache(n);
    return cache[n - 1];
}
