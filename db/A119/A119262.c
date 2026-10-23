/*
 * A119262  Number of B-trees of order infinity with n leaves, where a(n) =
 *          Sum_{k=1..floor(n/2)} a(k)*C(n-k-1,n-2*k) for n >= 2, with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A119262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/2⌋} a(k) * C(n-k-1, n-2k) (a(0)=0, a(1)=1)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binom.h"  /* provides binom(idx_t a, idx_t b) returning seq_t */

/* cached values of the sequence */
static seq_t *cache      = NULL;   /* array of a(i) */
static size_t cache_cap  = 0;        /* current capacity (in elements) */
static idx_t  cache_max  = 0;        /* highest index that has been computed */

/* ensure that cache has space for indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);           /* out of memory */
    /* zero newly allocated area */
    if (cache)
        memset(tmp + cache_cap, 0, (needed - cache_cap) * sizeof(seq_t));
    else
        memset(tmp, 0, needed * sizeof(seq_t));
    cache      = tmp;
    cache_cap  = needed;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    ensure_cache(n);

    if (cache_max < 1) {          /* initialise the first two terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t half = i / 2;
        for (idx_t k = 1; k <= half; ++k) {
            seq_t comb = binom(i - k - 1, i - 2 * k);
            sum += cache[k] * comb;
        }
        cache[i] = sum;
    }
    cache_max = n;
    return cache[n];
}
