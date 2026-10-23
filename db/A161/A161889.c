/*
 * A161889  a(1)=2. a(n) = the smallest integer > a(n-1) that is coprime to n,
 *          and is such that (a(n)-a(n-1)) is not a divisor of n.
 * https://oeis.org/A161889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2. For n > 1, a(n) = the smallest integer > a(n-1) that
 *            is coprime to n and such that (a(n)-a(n-1)) does *not* divide n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* grow cache capacity if needed */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed > 1 ? needed * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* initialise first term if required */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = 2;   /* a(1) */
        cache_len = 1;
    }

    /* compute terms up to n if not already cached */
    while (cache_len < n) {
        idx_t idx = (idx_t)(cache_len + 1);   /* 1‑based index of the new term */
        seq_t prev = cache[cache_len - 1];    /* a(n-1) */
        seq_t cand = prev + 1;
        seq_t sn   = (seq_t)idx;              /* n as seq_t for gcd and modulo */

        for (;;) {
            if (gcd(cand, sn) == (seq_t)1) {
                seq_t diff = cand - prev;     /* positive since cand > prev */
                if (diff != 0 && sn % diff != (seq_t)0)
                    break;                   /* found suitable candidate */
            }
            cand++;
        }

        grow_cache(cache_len + 1);
        cache[cache_len] = cand;
        cache_len++;
    }

    return cache[n - 1];
}
