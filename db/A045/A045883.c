/*
 * A045883  a(n) = ((3*n+1)*2^n - (-1)^n)/9.
 * https://oeis.org/A045883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3*n+1)*2^n - (-1)^n)/9 Recurrence: a(0)=0, a(1)=1,
 *            a(2)=3, a(n) = 3*a(n-1) - 4*a(n-3) for n ≥ 3 This implementation
 *            caches computed terms so that calls in any order are handled
 *            efficiently
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of a(n) values */
static size_t cache_cap   = 0;   /* number of slots allocated */
static idx_t  computed_max = 0; /* highest index that has been filled */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 3;   /* at least 3 slots */
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        computed_max = 2;
    }

    /* if requested value is already known, return it */
    if (n <= computed_max)
        return cache[n];

    /* grow cache if necessary */
    ensure_capacity(n);

    /* compute all missing values using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t3 = cache[i - 3];
        cache[i] = t1 * (seq_t)3 - t3 * (seq_t)4;
    }

    computed_max = n;
    return cache[n];
}
