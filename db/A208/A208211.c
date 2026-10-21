/*
 * A208211  a(n)=(a(n-1)^2*a(n-2)^4+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2 * a(n-2)^4 + 1) / a(n-3) with a(0) = a(1) =
 *            a(2) = 1 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms a(0), a(1), ... */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_size = 0;         /* filled entries */

/* Ensure the cache can hold indices up to `need-1`. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;    /* a(0), a(1), a(2) */

    size_t up_to = (size_t)n + 1;  /* we need indices 0..n inclusive */
    ensure_capacity(up_to);

    /* Initialize base cases if not yet done */
    if (cache_size < 3) {
        for (size_t i = cache_size; i < 3; ++i) cache[i] = (seq_t)1;
        cache_size = 3;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_size; i <= n; ++i) {
        seq_t a1 = cache[i-1];   /* a(i-1) */
        seq_t a2 = cache[i-2];   /* a(i-2) */
        seq_t a3 = cache[i-3];   /* a(i-3) */

        /* a1^2 */
        seq_t t1 = a1 * a1;
        /* a2^2 */
        seq_t t2 = a2 * a2;
        /* a2^4 = (a2^2)^2 */
        t2 = t2 * t2;
        /* product = a1^2 * a2^4 */
        seq_t prod = t1 * t2;
        /* numerator = product + 1 */
        prod = prod + (seq_t)1;
        /* a(i) = numerator / a3  (exact division) */
        cache[i] = prod / a3;
    }

    cache_size = up_to;
    return cache[(size_t)n];
}
