/*
 * A179596  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + x)/(1 - 2*x - 11*x^2 - 6*x^3).
 * https://oeis.org/A179596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=17; a(n)=2*a(n-1)+11*a(n-2)+6*a(n-3) for
 *            n>=3. This function memoises the sequence, works for any order
 *            of calls, and allocates the cache dynamically
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL; /* array holding the terms */
static size_t cache_size   = 0; /* allocated size of cache   */
static size_t computed_len = 0; /* number of terms already computed */

/* ensure cache can hold indices up to n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if (cache == NULL) {                          /* first allocation        */
        cache_size = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof (seq_t));
    } else if (cache_size <= (size_t)n) {          /* grow if needed          */
        size_t new_size = cache_size ? cache_size * 2 : 4;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof (seq_t));
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t
a(idx_t n)
{
    ensure_capacity(n);

    /* if we have already computed up to n, just return it */
    if (n < computed_len) {
        return cache[(size_t)n];
    }

    /* compute missing terms up to n */
    while (computed_len <= n) {
        seq_t val;
        if (computed_len == 0) {
            val = (seq_t)1;
        } else if (computed_len == 1) {
            val = (seq_t)3;
        } else if (computed_len == 2) {
            val = (seq_t)17;
        } else {
            val = cache[computed_len - 1] * (seq_t)2
                + cache[computed_len - 2] * (seq_t)11
                + cache[computed_len - 3] * (seq_t)6;
        }
        cache[computed_len] = val;
        computed_len++;
    }

    return cache[(size_t)n];
}
