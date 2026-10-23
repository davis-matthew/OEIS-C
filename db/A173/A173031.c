/*
 * A173031  Sequence whose G.f is given by: 1/(1-z)/(1-2*z)^2/(1-z-z^2).
 * https://oeis.org/A173031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=24, a(3)=79, a(4)=232;
 *            a(n+5)=6*a(n+4)-12*a(n+3)+7*a(n+2)+4*a(n+1)-4*a(n), for all n≥0.
 *            This recurrence generates OEIS A173031
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cache[0..cached_size-1] holds already computed values */
static idx_t cached_size = 0;  /* number of elements stored in cache */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cached_size) return;

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);            /* allocation failed */
    cache = tmp;

    /* Compute new entries from the current size up to n */
    for (idx_t i = cached_size; i < new_size; ++i) {
        if (i == 0)       cache[0] = (seq_t)1;
        else if (i == 1)  cache[1] = (seq_t)6;
        else if (i == 2)  cache[2] = (seq_t)24;
        else if (i == 3)  cache[3] = (seq_t)79;
        else if (i == 4)  cache[4] = (seq_t)232;
        else
            cache[i] =  6*cache[i-1]
                      -12*cache[i-2]
                      + 7*cache[i-3]
                      + 4*cache[i-4]
                      - 4*cache[i-5];
    }
    cached_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
