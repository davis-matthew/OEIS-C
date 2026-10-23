/*
 * A160705  Hankel transform of A052702.
 * https://oeis.org/A160705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-3) - 6*a(n-6) - 4*a(n-9) - a(n-12); Initial
 *            values: a(0)=a(1)=a(2)=a(3)=0, a(4)=a(5)=1, a(6)=-1,
 *            a(7)=a(8)=-4, a(9)=5, a(10)=a(11)=9
 * Range:     i8 n <= 23, i16 n <= 140, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised sequence values (defined for indices 0 .. cache_len-1) */
static idx_t cache_len   = 0;        /* number of computed entries in cache */
static idx_t cache_cap   = 0;        /* allocated capacity of cache array */

/* make sure values up to index n are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise with the given 12 base terms (index 0..11) */
        cache_cap = 13; /* allocate one more slot for the first computed term (a(12)) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return;  /* allocation failure – caller receives garbage */
        cache[0] = 0;  /* a(0) */
        cache[1] = 0;  /* a(1) */
        cache[2] = 0;  /* a(2) */
        cache[3] = 0;  /* a(3) */
        cache[4] = 1;  /* a(4) */
        cache[5] = 1;  /* a(5) */
        cache[6] = -1; /* a(6) */
        cache[7] = -4; /* a(7) */
        cache[8] = -4; /* a(8) */
        cache[9] = 5;  /* a(9) */
        cache[10] = 9; /* a(10) */
        cache[11] = 9; /* a(11) */
        cache_len = 12; /* values 0..11 are now computed */
    }

    if (n < cache_len) return;           /* we already have the answer */

    /* grow the array if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;        /* reallocation failure – caller receives garbage */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence holds for i >= 12; for i < 12 these terms are already known */
        seq_t val = (-4) * cache[i - 3]
                  + (-6) * cache[i - 6]
                  + (-4) * cache[i - 9]
                  + (-1) * cache[i -12];
        cache[i] = val;
    }
    cache_len = n + 1; /* now we have values up to n included */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
