/*
 * A077913  Expansion of 1/((1-x)*(1+x+2*x^2+x^3)).
 * https://oeis.org/A077913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=-1, a(3)=1, for n≥4: a(n) = -a(n-2) +
 *            a(n-3) + a(n-4)
 * Range:     i8 n <= 19, i16 n <= 39, i32 n <= 77, i64 n <= 157,
 *            i128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache so that calls to a() may
   arrive in any order.  The cache is resized with realloc, and the
   code is suitable for both integral and arbitrary‑precision seq_t
   types. */

#include <stdlib.h>

/* static cache to allow repeated calls in any order */
static seq_t *cache = NULL;
static size_t cache_size = 0;

static void
ensure_cache(idx_t n)
{
    /* need indices 0..n */
    if ((size_t)n < cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    if (new_size < 16)
        new_size = 16;
    else if (new_size < cache_size * 2)
        new_size = cache_size * 2;

    /* realloc must be cast to the correct pointer type for C++ */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure: abort */
        abort();
    }
    cache = new_cache;

    /* initialise new elements */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;      /* a(0) */
        if (new_size > 1) cache[1] = (seq_t)0;   /* a(1) */
        if (new_size > 2) cache[2] = (seq_t)(-1);/* a(2) */
        if (new_size > 3) cache[3] = (seq_t)1;   /* a(3) */
    }
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < 4) {
            /* base values already set */
        } else {
            seq_t v = -cache[i-2] + cache[i-3] + cache[i-4];
            cache[i] = v;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
