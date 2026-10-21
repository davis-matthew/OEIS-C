/*
 * A078001  Expansion of (1-x)/(1-2*x+x^2+x^3).
 * https://oeis.org/A078001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 58, i64 n <= 115,
 *            i128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=1,
   a(n)=2*a(n-1)-a(n-2)-a(n-3) for n≥3 */
/* This implements OEIS A078001 (expansion of (1-x)/(1-2x+x²+x³)) */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0..cached_up_to] */
static idx_t cached_up_to = 0;       /* largest n currently computed */

/* Grow the cache to at least new_n (inclusive) */
static void
grow_cache(idx_t new_n)
{
    if (new_n <= cached_up_to)
        return;
    size_t new_size = (size_t)(new_n + 1);
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* initial values for n=0,1,2 if not already set */
    if (new_n >= 0 && cached_up_to < 0) { /* never occurs, just for symmetry */
    }
    /* fill missing entries if expanding beyond current cache */
    for (idx_t i = cached_up_to + 1; i <= new_n; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[i] = 1;
        } else {
            /* recurrence: a(i)=2*a(i-1)-a(i-2)-a(i-3) */
            cache[i] = 2 * cache[i - 1] - cache[i - 2] - cache[i - 3];
        }
    }
    cached_up_to = new_n;
}

seq_t a(idx_t n)
{
    /* first time: allocate and initialize */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = 1;           /* a(0)=a(1)=a(2)=1 */
        cached_up_to = 2;
    }
    if (n <= cached_up_to)
        return cache[n];
    grow_cache(n);
    return cache[n];
}
