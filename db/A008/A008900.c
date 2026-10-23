/*
 * A008900  x->x/2 if x even, x->3x-1 if x odd.
 * https://oeis.org/A008900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 89; for n > 0, a(n) = a(n‑1)/2 if a(n‑1) even, otherwise
 *            a(n) = 3*a(n‑1)‑1
 * Range:     u8 n <= 0, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array holding computed terms */
static size_t cache_cap = 0;       /* current allocated capacity (number of elements) */
static idx_t cached_up_to = 0;     /* largest index that has been computed */

/* Compute the sequence up to index n, extending the cache as needed. */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        /* first allocation with a(0) = 89 */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)89;
        cache_cap = 1;
        cached_up_to = 0;
    }

    if (n <= cached_up_to) return;        /* already computed */

    /* extend the array to hold indices up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;

    /* compute missing values sequentially */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) == 0)      /* even */
            prev = prev / 2;
        else                           /* odd */
            prev = (seq_t)3 * prev - 1;
        cache[i] = prev;
    }
    cached_up_to = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
