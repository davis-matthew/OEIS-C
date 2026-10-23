/*
 * A160261  Numerator of Hermite(n, 13/29).
 * https://oeis.org/A160261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A160261
 * Formula: a(0)=1, a(1)=26, a(n)=26*a(n-1)-(n-1)*1682*a(n-2) for n>=2
 * (Derived from the D-finite recurrence: a(n)-26*a(n-1)+1682*(n-1)*a(n-2)=0)
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static size_t  last_computed = 0;

/* Ensure the cache can hold values up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;   /* double until sufficient */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* If memory allocation fails, we simply abort – the sequence
           tests should never need that many elements. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)26;

    ensure_cache_size(n);

    /* Initialise base cases if not yet set */
    if (cache_cap >= 2 && cache[0] == 0 && cache[1] == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        last_computed = 1;
    }

    /* Extend computed range if necessary */
    if (last_computed < (size_t)n) {
        for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
            /* a(i) = 26*a(i-1) - (i-1)*1682*a(i-2) */
            seq_t term1 = (seq_t)26 * cache[i - 1];
            seq_t term2 = (seq_t)(i - 1) * (seq_t)1682 * cache[i - 2];
            cache[i] = term1 - term2;
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
