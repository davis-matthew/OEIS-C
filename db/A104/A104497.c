/*
 * A104497  Expansion of sqrt(1-8x)/sqrt(1-4x).
 * https://oeis.org/A104497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   n*a(n) = (12*n-14)*a(n-1) - 32*(n-2)*a(n-2), a(0)=1, a(1)=-2
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cached values of the sequence; supports any order of calls */
static seq_t *cache = NULL;       /* array of a(0) … a(k) */
static idx_t cache_size = 0;      /* number of values stored */

/* grow the cache so that a(0)…a(n) are available */
static void ensure_cache(idx_t n)
{
    /* First initialisation: cache a(0) and a(1) */
    if (cache == NULL) {
        size_t init = 2;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (cache == NULL) return; /* out of memory – caller may handle */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache_size = 2;
    }

    /* nothing to do if already computed */
    if (n < cache_size) return;

    /* grow the storage if needed */
    size_t new_size = cache_size;
    while (new_size <= (size_t)n) new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return; /* out of memory – caller may handle */
    cache = tmp;

    /* compute missing terms using the recurrence */
    idx_t start = cache_size;
    if (start < 2) start = 2;
    for (idx_t i = start; i <= n; ++i) {
        seq_t ci = (seq_t)i;
        seq_t term1 = ((seq_t)12 * ci - (seq_t)14) * cache[i-1];
        seq_t term2 = (seq_t)32 * ((seq_t)(i-2)) * cache[i-2];
        cache[i] = (term1 - term2) / ci;
    }

    cache_size = n + 1;
}

/* exported sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
