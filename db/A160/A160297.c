/*
 * A160297  Numerator of Hermite(n, 23/30).
 * https://oeis.org/A160297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15^n * Hermite(n, 23/30)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies a(0)=1, a(1)=23 and for n≥2 */
/*   a(n) = 23*a(n-1) - 450*(n-1)*a(n-2) */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void init_cache(void)
{
    cache_size = 2;
    cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
    cache[0] = (seq_t)1;          /* a(0) */
    cache[1] = (seq_t)23;         /* a(1) */
}

static void grow_cache(idx_t n)
{
    if (cache == NULL)
        init_cache();

    while (cache_size <= n) {
        idx_t i     = cache_size;    /* index to compute */
        idx_t prev  = i - 1;          /* a(i-1) */
        idx_t prev2 = i - 2;          /* a(i-2) */

        /* recurrence: a(i) = 23*a(i-1) - 450*(i-1)*a(i-2) */
        seq_t term = (seq_t)23 * cache[prev]
                   - (seq_t)450 * (seq_t)prev * cache[prev2];

        cache = (seq_t*)realloc(cache, sizeof(seq_t) * (cache_size + 1));
        cache[cache_size] = term;
        cache_size++;
    }
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[n];
}
