/*
 * A180037  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1+x)/(1-5*x-2*x^2).
 * https://oeis.org/A180037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2); a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses memoised recurrence; works for any seq_t type */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (works for out‑of‑order calls) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current allocated capacity */
    static size_t cache_len = 0;   /* next index to compute (cache_len = last_computed_index+1) */

    /* first call – allocate at least two elements */
    if (cache == NULL) {
        /* user may ask for n = 0,1,2, ... ; allocate enough to hold up to n */
        cache_cap = (n >= 1 ? n + 1 : 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            abort();              /* out of memory – halt evaluation */
        }
        cache_len = 0;            /* nothing computed yet */
    }

    /* enlarge cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= (size_t)n + 1) {
            new_cap <<= 1;       /* double until big enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();              /* out of memory – halt evaluation */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute the missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)6;
        } else {
            cache[i] = cache[i - 1] * (seq_t)5 + cache[i - 2] * (seq_t)2;
        }
    }
    cache_len = (size_t)n + 1;      /* next free index after the last computed one */

    return cache[(size_t)n];
}
