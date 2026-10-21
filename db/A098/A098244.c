/*
 * A098244  First differences of Chebyshev polynomials S(n,171) = A097844(n)
 *          with Diophantine property.
 * https://oeis.org/A098244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 171*a(n-1) - a(n-2), a(0)=1, a(1)=170
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised recurrence; computation up to any n is performed only once, */
/* making the function O(n) total time for all calls.  It works for the */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>   /* realloc, abort */

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap   = 0;    /* current capacity of the array */

/* Resize the cache so it can hold at least 'need' entries. */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap < need)
        new_cap <<= 1;                 /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                         /* allocation failed – should not happen */
        abort();

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(idx_t n). */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;           /* n fits in size_t locally (n ≤ 10000) */

    if (idx >= cache_size) {
        ensure_cap(idx + 1);

        for (size_t i = cache_size; i <= idx; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)170;
            else
                cache[i] = cache[i - 1] * (seq_t)171 - cache[i - 2];
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
