/*
 * A047974  a(n) = a(n-1) + 2*(n-1)*a(n-2).
 * https://oeis.org/A047974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A047974:  a(n) = a(n-1) + 2*(n-1)*a(n-2)
 * Offset : 0   (a(0) = 1, a(1) = 1)
 *
 * The implementation uses a static memoisation table and expands it
 * on demand.  It works for any unsigned integer type or arbitrary‑
 * precision type that supports the standard arithmetic operators.
 */
#include <stdlib.h>

/* static storage for memoised terms */
static seq_t *cache = NULL;      /* array of terms */
static idx_t cache_size = 0;     /* number of entries currently allocated */

/* ensure that cache contains indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)          /* already computed */
        return;

    idx_t new_size = n + 1;      /* we need indices 0..n inclusive */

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!tmp && new_size > 0)    /* allocation failure – abort */
        abort();

    cache = tmp;

    /* initialise base cases that may be missing when expanding */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
    }
    if (cache_size <= 1 && new_size > 1) {
        cache[1] = (seq_t)1;
    }

    /* compute new entries using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 2)                /* base cases already set */
            continue;

        /* a(i) = a(i-1) + 2*(i-1)*a(i-2) */
        seq_t term = cache[i - 2] * ((seq_t)2 * (seq_t)(i - 1));
        cache[i] = cache[i - 1] + term;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
