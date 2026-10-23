/*
 * A037512  Base 3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; a(n)=3*a(n-1)+d(n-1) where d(k) = {2,0,1}[k mod 3] */
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array */
static size_t cache_len = 0;     /* number of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */

static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = needed > cache_cap ? needed
                        : (cache_cap * 2 > 0 ? cache_cap * 2 : 8);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {        /* allocation failed */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in normal use */
    size_t nn = (size_t)n;

    if (cache_len < nn) {
        ensure_capacity(nn);
        for (size_t i = cache_len; i < nn; ++i) {
            unsigned int digit;
            switch (i % 3) {
                case 0: digit = 2; break;
                case 1: digit = 0; break;
                case 2: digit = 1; break;
            }
            if (i == 0) {
                cache[0] = (seq_t)digit;
            } else {
                cache[i] = cache[i-1] * 3 + (seq_t)digit;
            }
        }
        cache_len = nn;
    }
    return cache[nn-1];
}
