/*
 * A075411  Squares of A002276.
 * https://oeis.org/A075411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) with a(0)=0,
 *            a(1)=4, a(2)=484. This computes A075411
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* array of computed terms */
static idx_t  cache_len   = 0;      /* current allocated length */
static idx_t  cache_max   = (idx_t)-1;  /* largest index whose value is set */

static void seq_ensure(idx_t need)
/* Ensure the cache holds at least `need` elements. */
{
    if (need <= cache_len)
        return;
    seq_t *tmp = (seq_t *)realloc(seq_cache, need * sizeof(seq_t));
    seq_cache = tmp;
    cache_len = need;
}

seq_t a(idx_t n)
/* Return A075411(n).  The values are memoised in static storage
   and are computed on demand. */
{
    /* initialise the first three terms if necessary */
    if (seq_cache == NULL) {
        seq_ensure(3);
        seq_cache[0] = 0;
        seq_cache[1] = 4;
        seq_cache[2] = 484;
        cache_max = 2;
    }

    /* already computed?  */
    if (n <= cache_max)
        return seq_cache[n];

    /* grow the cache to hold the requested index */
    seq_ensure(n + 1);

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = seq_cache[i - 1] * (seq_t)111;
        seq_t t2 = seq_cache[i - 2] * (seq_t)1110;
        seq_t t3 = seq_cache[i - 3] * (seq_t)1000;
        seq_cache[i] = t1 - t2 + t3;
    }
    cache_max = n;
    return seq_cache[n];
}
