/*
 * A088381  Numbers greater than the cube of their smallest prime factor.
 * https://oeis.org/A088381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are integers m such that m > (smallest prime factor of m)^3
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_cap = 0;         /* capacity of cache */
static size_t cache_len = 0;         /* number of terms currently cached */
static uint64_t last_scanned = 9;     /* last integer examined (init to 9) */

/* Ensure cache has at least 'need' capacity */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; this case never used */

    /* return cached result if already computed */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* generate terms until we reach the nth */
    for (uint64_t m = last_scanned + 1; ; ++m)
    {
        last_scanned = m;                     /* update first */
        uint64_t lpf64 = _smallest_prime_factor(m);

        /* compute lpf^3 using 128‑bit to avoid overflow on 64‑bit build */
        unsigned __int128 cube = (unsigned __int128)lpf64 * lpf64 * lpf64;

        if ((unsigned __int128)m > cube)
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
            if (cache_len >= (size_t)n)
                break;
        }
    }

    return cache[n - 1];
}
