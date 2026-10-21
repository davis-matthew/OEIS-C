/*
 * A078451  a(1)=1, a(n) = ceiling(n/a(a(n-1))).
 * https://oeis.org/A078451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=ceil(n/a(a(n-1)))
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t max_cached = 0;
static size_t cache_cap = 0;

static void ensure_cache_size(idx_t n)
{
    /* Ensure space for indices 0 .. n (index 0 unused) */
    size_t needed = (size_t)(n + 1);
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void compute_upto(idx_t n)
{
    if (n <= max_cached)
        return;

    ensure_cache_size(n);

    if (max_cached == 0) {
        /* Initialize first term */
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        /* a(a(i-1)) */
        seq_t prev_val = cache[i - 1];
        idx_t idx = (idx_t)prev_val;      /* a(i-1) fits into idx_t */
        seq_t denom = cache[idx];
        seq_t num = (seq_t)i;

        seq_t q = num / denom;
        if (num % denom != (seq_t)0)
            ++q;
        cache[i] = q;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, return 0 as a guard */
    compute_upto(n);
    return cache[n];
}
