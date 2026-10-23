/*
 * A152188  a(0) = 0, a(n) is the number of divisors of n that are greater
 *          than a(n-1).
 * https://oeis.org/A152188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5384, u16 n <= 9104, u32 n <= 9104, u64 n <= 9104,
 *            u128 n <= 9104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts divisors of n that exceed a(n‑1)
   a(0) = 0
   a(n) = |{ d | d divides n and d > a(n‑1) }| for n ≥ 1 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of a(n) */
static idx_t  cache_len = 0;  /* number of values cached (>= 1, 0 means uninitialised) */

/* Return a(n) for any non‑negative n, respecting the definition above. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first term a(0)=0 if necessary. */
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)          /* allocation failure – give a safe result */
            return 0;
        cache[0] = 0;
        cache_len = 1;
    }

    /* If the value is already cached, return it straight away. */
    if (n < cache_len)
        return cache[n];

    /* Expand the cache array to hold indices up through n. */
    idx_t old_len = cache_len;          /* index of first new element */
    idx_t new_len = n + 1;              /* length after expansion */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return 0;                       /* allocation failure – safe result */
    cache = tmp;
    cache_len = new_len;

    /* Compute a(k) for all new indices k ∈ [old_len .. n] inclusive. */
    for (idx_t k = old_len; k <= n; ++k)
    {
        seq_t prev = cache[k - 1];   /* a(k-1) */
        seq_t cnt  = 0;              /* number of qualifying divisors of k */

        /* Enumerate divisor pairs (d, k/d).  Only loop until d*d ≤ k. */
        for (idx_t d = 1; d <= k / d; ++d)
        {
            if (k % d == 0)
            {
                seq_t d1 = (seq_t)d;
                seq_t d2 = (seq_t)(k / d);

                if (d1 > prev) ++cnt;
                /* count the paired divisor if it's distinct from d1 */
                if (d1 != d2 && d2 > prev) ++cnt;
            }
        }

        cache[k] = cnt;
    }

    return cache[n];
}
