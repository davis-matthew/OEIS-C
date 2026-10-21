/*
 * A052932  Expansion of (1-x)/(1 - 2*x - x^4 + x^5).
 * https://oeis.org/A052932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-4) - a(n-5) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=9
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* array of computed values         */
static idx_t  cache_cap   = 0;       /* allocated size (in elements)     */
static idx_t  cache_max   = 0;       /* highest index with a value set   */

/* Make sure the cache can hold indices up to n.
   On first use it also stores the initial five terms. */
static void
seq_cache_ensure(idx_t n)
{
    if (n < cache_cap)
        return;

    /* grow capacity (double each time, at least 5) */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 5;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* fatal if allocation fails */
    seq_cache = tmp;
    cache_cap = new_cap;

    /* initialise the first five terms if not already done */
    if (cache_max < 4) {
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)2;
        seq_cache[3] = (seq_t)4;
        seq_cache[4] = (seq_t)9;
        cache_max   = 4;
    }
}

seq_t a(idx_t n)
{
    /* make sure cache is large enough */
    seq_cache_ensure(n);

    if (n <= cache_max)
        return seq_cache[n];

    /* compute sequence up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_cache[i] = 2 * seq_cache[i - 1] + seq_cache[i - 4] - seq_cache[i - 5];
    }
    cache_max = n;
    return seq_cache[n];
}
