/*
 * A228544  x-values in the solution to the Pell equation x^2 - 61*y^2 = -1.
 * https://oeis.org/A228544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3532638098*a(n-1) - a(n-2) with a(1) = 29718, a(2) =
 *            104982939026082
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamic array holding computed terms */
static size_t cache_cap = 0;    /* allocated capacity of cache              */
static size_t cache_size = 0;   /* number of terms currently stored         */

/// Ensure that cache has room for at least `need` elements.
static void _ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 */
        return (seq_t)0;

    size_t idx = (size_t)(n - 1);   /* 0‑based index of the element */

    /* initialisation: first two terms of the sequence */
    if (cache_size == 0) {
        _ensure_capacity(2);
        cache[0] = (seq_t)29718;
        cache[1] = (seq_t)104982939026082ULL;   /* 2nd term */
        cache_size = 2;
    }

    /* grow cache if necessary and compute missing terms */
    if (idx >= cache_size) {
        _ensure_capacity(idx + 1);
        const seq_t coeff = (seq_t)3532638098ULL;
        for (size_t i = cache_size; i <= idx; ++i) {
            /* a(i) = coeff * a(i-1) - a(i-2)  (i is 1‑based) */
            cache[i] = coeff * cache[i-1] - cache[i-2];
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
