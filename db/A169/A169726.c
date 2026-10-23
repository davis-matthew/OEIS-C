/*
 * A169726  a(n) = 3*2^n*(2^n-1) + 1.
 * https://oeis.org/A169726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n*(2^n-1)+1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;        /* array of computed terms */
static idx_t  cache_len = 0;       /* allocated length of the array          */
static idx_t  cache_count = 0;     /* number of terms already computed     */
static bool   cache_initialised = false;

/* ensure that the cache has room for at least "needed" elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_len)
        return;
    idx_t new_len = cache_len ? cache_len : 1;
    while (new_len < needed)
        new_len <<= 1;
    seq_t *new_cache = seq_alloc(new_len);
    /* copy existing values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    /* keep old block (tiny leak but harmless) */
    cache = new_cache;
    cache_len = new_len;
}

/* initialise the first three terms */
static void init_cache(void)
{
    ensure_capacity(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)37;
    cache_count = 3;
    cache_initialised = true;
}

seq_t a(idx_t n)
{
    if (!cache_initialised)
        init_cache();

    /* compute terms up to n if necessary */
    if (n >= cache_count) {
        ensure_capacity(n + 1);
        for (idx_t i = cache_count; i <= n; ++i) {
            seq_t term =
                (seq_t)7 * cache[i - 1]
                - (seq_t)14 * cache[i - 2]
                + (seq_t)8  * cache[i - 3];
            cache[i] = term;
        }
        cache_count = n + 1;
    }

    return cache[n];
}
