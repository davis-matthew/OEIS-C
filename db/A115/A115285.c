/*
 * A115285  Diagonal sums of correlation triangle for 1,3,4,4,4,...(A113311).
 * https://oeis.org/A115285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n ≥ 7 * Base
 *            values: a(0)=1, a(1)=3, a(2)=7, a(3)=14, a(4)=23, a(5)=35,
 *            a(6)=50 * (diagonal sums of correlation triangle for
 *            1,3,4,4,4,... (A113311)) * Generates OEIS sequence A115285
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;  /* cached sequence values */
static idx_t cache_size = 0U;/* number of values already computed */

/* ensure the cache contains at least the terms 0..n */
static void
ensure_cache(idx_t n)
{
    /* we always keep room for the first 7 terms */
    idx_t needed = n + 1;
    if (needed < 7U)
        needed = 7U;

    if (needed <= cache_size)
        return;

    cache = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
    if (!cache)
        abort();

    /* initialise the first seven terms on the first allocation */
    if (cache_size == 0U) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)23;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)50;
        cache_size = 7U;
    }

    /* compute further terms using the recurrence */
    for (idx_t i = cache_size; i < needed; ++i) {
        seq_t val = cache[i-1] + cache[i-2];
        val -= cache[i-4];
        val -= cache[i-5];
        val += cache[i-6];
        cache[i] = val;
    }
    cache_size = needed;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
