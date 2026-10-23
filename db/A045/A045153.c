/*
 * A045153  Numbers whose base-4 representation contains exactly four 2's and
 *          no 3's.
 * https://oeis.org/A045153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   The sequence contains integers whose base‑4 representation * has
 *            exactly four digits equal to 2 and no digit equal to 3. * The
 *            function produces terms by scanning integers in increasing order
 *            * and caching the found terms
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoization */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  current   = 0;

/* Return true iff v has exactly four 2's and no 3's in base‑4. */
static inline bool satisfies(idx_t v)
{
    idx_t t   = v;
    int   cnt = 0;
    while (t) {
        uint8_t d = (uint8_t)(t & 3);   /* lowest base‑4 digit */
        if (d == 3)
            return false;
        if (d == 2) {
            ++cnt;
            if (cnt > 4)
                return false;
        }
        t >>= 2;                       /* divide by 4 */
    }
    return cnt == 4;
}

/* Resize cache if necessary. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap : 4;
    while (cap < needed)
        cap <<= 1;
    seq_t *np = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!np)
        abort();
    cache = np;
    cache_cap = cap;
}

/* External entry point. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; n>=1 in valid calls */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++current;
        if (satisfies(current)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)current;
        }
    }
    return cache[n - 1];
}
