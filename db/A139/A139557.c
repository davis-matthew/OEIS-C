/*
 * A139557  a(1)=2. For n>=2, a(n) = smallest integer > a(n-1) that is not
 *          coprime to n but is coprime to (n+1).
 * https://oeis.org/A139557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer > a(n-1) that is not coprime to n but is
 *            coprime to (n+1). a(1) = 2. The function uses memoisation to
 *            allow arbitrary order of calls
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;       /* array of computed a(n) values */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* Grow cache storage to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) { /* allocation failed */
        exit(EXIT_FAILURE);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n=0, but return 0 to avoid UB */
        return (seq_t)0;

    /* Ensure cache can hold the requested index. */
    grow_cache(n);

    /* Initialise first term if cache empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;   /* a(1) = 2 */
        cache_len = 1;
    }

    /* If value already computed, return it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + 1;
        seq_t sn   = (seq_t)k;   /* n as seq_t */
        seq_t sn1  = sn + 1;

        /* Find the smallest candidate satisfying the conditions. */
        for (;;) {
            seq_t g1 = gcd(cand, sn);
            if (g1 != (seq_t)1) {
                seq_t g2 = gcd(cand, sn1);
                if (g2 == (seq_t)1)
                    break;
            }
            ++cand;
        }
        cache[cache_len] = cand;
        ++cache_len;
    }
    return cache[n - 1];
}
