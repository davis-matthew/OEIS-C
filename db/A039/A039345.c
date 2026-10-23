/*
 * A039345  Numbers whose base-8 representation has the same nonzero number of
 *          2's and 6's.
 * https://oeis.org/A039345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑8 representation *
 *            contains the same non‑zero number of digits 2 and 6. * The
 *            function generates the increasing list of such numbers on demand
 *            * and caches the results for future calls
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Static cache for generated terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold at least 'need' elements, reallocating if necessary */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff x has equal, non‑zero counts of digits 2 and 6 in base 8 */
static bool ok(seq_t x)
{
    if (x == 0)
        return false;
    unsigned cnt2 = 0, cnt6 = 0;
    seq_t t = x;
    while (t > 0)
    {
        unsigned d = (unsigned)(t % 8);
        if (d == 2)
            ++cnt2;
        else if (d == 6)
            ++cnt6;
        t /= 8;
    }
    return cnt2 > 0 && cnt2 == cnt6;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* behavior for indices below offset is irrelevant */

    /* Generate terms until we have at least n of them */
    if (cache_len < (size_t)n)
    {
        grow_cache((size_t)n);
        seq_t start = cache_len == 0 ? (seq_t)0 : cache[cache_len - 1] + 1;
        for (seq_t val = start; cache_len < (size_t)n; ++val)
        {
            if (ok(val))
                cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
