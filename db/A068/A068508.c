/*
 * A068508  a(n) = round((a(n-1) + a(n-2))/a(n-3)) starting with
 *          a(1)=a(2)=a(3)=1.
 * https://oeis.org/A068508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((a(n-1)+a(n-2))/a(n-3)), a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached sequence values, 1‑based */
static idx_t  cache_len = 0;       /* number of terms already computed */
static idx_t  cache_cap = 0;       /* capacity of the cache (in terms) */

/* Ensure the cache can hold at least 'need' terms (1‑based indexing). */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (!new_cache)
        abort();          /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n < 1, return 0 as a safe fallback */

    /* initialise cache if this is the first call */
    if (!cache)
    {
        cache = (seq_t *)calloc(5, sizeof(seq_t));  /* small initial size */
        cache_cap = 4;
        /* base terms */
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        cache_len = 3;
    }

    /* grow the cache if needed */
    ensure_cache_cap(n);

    /* compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i <= 3)
        {
            cache[i] = (seq_t)1;  /* base values */
            continue;
        }

        /* recurrence: round((a(i-1)+a(i-2))/a(i-3)) */
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t denom = cache[i - 3];
        seq_t half = denom / 2;                      /* floor(denom/2) */
        cache[i] = (sum + half) / denom;             /* rounding to nearest int, ties up */
    }

    cache_len = n;
    return cache[n];
}
