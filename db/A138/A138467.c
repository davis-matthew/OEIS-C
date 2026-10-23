/*
 * A138467  a(1)=1, then for n>=2 a(n) = n - floor((1/3)*a(a(n-1))).
 * https://oeis.org/A138467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2: a(n) = n - floor(a(a(n-1))/3)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] unused, cache[1] = 1 */
static idx_t cache_cap = 0;        /* number of elements allocated */
static idx_t cached_max   = 0;     /* largest index computed */

/* Ensure that the cache can hold indices up to max_n.
 * Compute any new values using the recurrence. */
static void ensure_cache(idx_t max_n)
{
    if (max_n >= cache_cap) {
        idx_t new_cap = (max_n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(1)=1 if not already done */
    if (cached_max == 0) {
        cache[1] = (seq_t)1;
        cached_max = 1;
    }

    /* compute values from cached_max+1 up to max_n */
    for (idx_t i = cached_max + 1; i <= max_n; ++i) {
        idx_t prev_idx = (idx_t)cache[i - 1];          /* a(i-1) */
        seq_t inner_value = cache[prev_idx];          /* a( a(i-1) ) */
        seq_t div3 = inner_value / ((seq_t)3);        /* floor division */
        cache[i] = (seq_t)i - div3;
    }
    cached_max = max_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, guard against accident */
    ensure_cache(n);
    return cache[n];
}
