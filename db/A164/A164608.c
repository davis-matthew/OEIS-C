/*
 * A164608  Expansion of (1+4*x)/(1-8*x+8*x^2).
 * https://oeis.org/A164608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 8*a(n-2), a(0)=1, a(1)=12 */
#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms */
static idx_t cache_len = 0;       /* largest index computed so far */
static size_t cache_cap = 0;      /* current array capacity */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        size_t cap = (size_t)(n + 1);
        if (cap < 2) cap = 2;                     /* at least slots 0 and 1 */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache_cap = cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache_len = 1;                            /* indices 0 and 1 are known */
    }

    /* extend cache if needed */
    if (n > cache_len) {
        if (n >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)8 * cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n];
}
