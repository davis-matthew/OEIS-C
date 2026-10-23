/*
 * A008546  Quintuple factorial numbers: Product_{k = 0..n-1} (5*k + 4).
 * https://oeis.org/A008546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k = 0..n-1} (5*k + 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (also a(n) = (5*n-1)!!_5, with a(0) = 1)       */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values   */
static size_t cache_len = 0;     /* allocated capacity         */
static idx_t  cached_max = 0;    /* highest index computed     */

/* ensure cache has room for indices up to 'needed' (inclusive) */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_len)
        return;
    size_t newcap = (cache_len > 0) ? cache_len : 16;
    while (newcap < needed)
        newcap <<= 1;                     /* double until big enough */
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    /* In a production system we would check newptr == NULL  */
    cache = newptr;
    cache_len = newcap;
}

/* compute a(n)  */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* first call : allocate and fill up to n */
    if (cache == NULL) {
        ensure_capacity((size_t)n + 1);
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t mult = (seq_t)(5 * i - 1);          /* 5*(i-1)+4  */
            cache[i] = cache[i - 1] * mult;
        }
        cached_max = n;
        return cache[n];
    }

    /* extend cache if requested index larger than current */
    if (n > cached_max) {
        ensure_capacity((size_t)n + 1);
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t mult = (seq_t)(5 * i - 1);
            cache[i] = cache[i - 1] * mult;
        }
        cached_max = n;
    }

    /* return the cached value (also works for n <= cached_max) */
    return cache[n];
}
