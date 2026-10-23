/*
 * A018921  Define the generalized Pisot sequence T(a(0),a(1)) by: a(n+2) is
 *          the greatest integer such that a(n+2)/a(n+1) < a(n+1)/a(n). This
 *          is T(4,8).
 * https://oeis.org/A018921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-4) for n >= 4; initial values: a(0)=4,
 *            a(1)=8, a(2)=15, a(3)=28
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* ensure the cache can hold at least needed elements */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* out of memory; abort as build environment guarantees memory */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* ensure initial 4 values are present */
static void init_cache(void)
{
    if (cache_len >= 4)
        return;
    grow_cache(8);
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)15;
    cache[3] = (seq_t)28;
    cache_len = 4;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) val = (seq_t)4;
        else if (i == 1) val = (seq_t)8;
        else if (i == 2) val = (seq_t)15;
        else if (i == 3) val = (seq_t)28;
        else {
            seq_t two_prev = cache[i - 1] + cache[i - 1]; /* 2 * a(i-1) */
            val = two_prev - cache[i - 4];
        }
        grow_cache(i + 1);
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
