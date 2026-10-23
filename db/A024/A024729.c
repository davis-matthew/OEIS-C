/*
 * A024729  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(2)=1 and a(3)=5.
 * https://oeis.org/A024729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1, a(3)=5; a(n)=∑_{i=1..⌊(n+1)/4⌋} a(2*i-1)*a(n-2*i+1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

/* static cache: 1‑based indexing, cache[0] unused */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;      /* largest computed index */
static size_t cache_cap  = 0;      /* capacity of cache (elements) */

/* Ensure values up to n (inclusive) are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* Resize if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails we simply abort; the evaluated environment will handle it */
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1 || k == 2) {
            val = 1;
        } else if (k == 3) {
            val = 5;
        } else {
            idx_t max_i = (k + 1) / 4;   /* floor((k+1)/4) */
            seq_t sum = 0;
            for (idx_t i = 1; i <= max_i; ++i) {
                idx_t odd   = 2 * i - 1;
                idx_t other = k - 2 * i + 1;
                sum += cache[odd] * cache[other];
            }
            val = sum;
        }
        cache[k] = val;
    }
    cache_size = n;
}

/* Sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, behaviour for 0 irrelevant */
    ensure_cache(n);
    return cache[n];
}
