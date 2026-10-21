/*
 * A079980  Number of permutations of length 2n satisfying -k<=p(i)-i<=r and
 *          p(i)-i not in I, i=1..2n, with k=3, r=3, I={-2,0,1,2}. There is no
 *          one such permutation of length 2n+1.
 * https://oeis.org/A079980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+4*a(n-3)+2*a(n-4)+2*a(n-5)-2*a(n-6)+a(n-7)+a(n-9)
 *            for n≥9 Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=3,
 *            a(5)=8, a(6)=12, a(7)=27, a(8)=52
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation cache */
static idx_t cache_sz = 0;    /* number of elements stored (indices 0 … cache_sz-1) */

/* Initialise cache with the first 9 terms.  Called only on first use. */
static void init_cache(void)
{
    const uint8_t init_vals[9] = {1,0,1,2,3,8,12,27,52};
    cache_sz = 9;
    cache = (seq_t*)malloc(sizeof(seq_t) * cache_sz);
    for (idx_t i = 0; i < cache_sz; ++i)
        cache[i] = (seq_t)init_vals[i];
}

static void expand_cache(idx_t up_to)
{
    if (up_to + 1 > cache_sz) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (up_to + 1));
        cache = tmp;
        for (idx_t i = cache_sz; i <= up_to; ++i)
            cache[i] = (seq_t)0;
        cache_sz = up_to + 1;
    }
}

seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (cache_sz == 0)
        init_cache();

    /* return if already known */
    if (n < cache_sz)
        return cache[n];

    /* compute missing terms up to n */
    expand_cache(n);
    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = cache[i-2];
        val += cache[i-3] * (seq_t)4;
        val += cache[i-4] * (seq_t)2;
        val += cache[i-5] * (seq_t)2;
        val -= cache[i-6] * (seq_t)2;
        val += cache[i-7];
        val += cache[i-9];
        cache[i] = val;
    }
    return cache[n];
}
