/*
 * A042032  Numerators of continued fraction convergents to sqrt(540).
 * https://oeis.org/A042032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 238142*a(n-8) - a(n-16)
   Initial terms (n = 0 … 15):
   23, 93, 395, 488, 5275, 5763, 28327, 119071,
   5505593, 22141443, 94071365, 116212808,
   1256199445, 1372412253, 6745848457, 28355806081
*/

#include <stdlib.h>

/* Static cache for computed values. */
static seq_t *cache      = NULL;   /* array of terms                           */
static idx_t  cache_len  = 0;      /* number of currently initialized terms    */
static idx_t  cache_cap  = 0;      /* allocated capacity (>= cache_len)        */

/* Initialise the cache with the first 16 known terms. */
static void init_cache(void)
{
    if (cache_len != 0) return;   /* already initialised */

    static const seq_t base[16] = {
        23ULL, 93ULL, 395ULL, 488ULL,
        5275ULL, 5763ULL, 28327ULL, 119071ULL,
        5505593ULL, 22141443ULL, 94071365ULL, 116212808ULL,
        1256199445ULL, 1372412253ULL, 6745848457ULL, 28355806081ULL
    };

    /* Allocate space for 16 terms. */
    cache = (seq_t *)malloc(16 * sizeof(seq_t));
    if (!cache) return;    /* allocation failure – nothing to do */

    for (idx_t i = 0; i < 16; ++i) cache[i] = base[i];

    cache_len = 16;
    cache_cap = 16;
}

/* Ensure the cache has capacity at least (n+1). */
static void ensure_cache_len(idx_t n)
{
    if (n + 1 <= cache_cap) return;        /* already large enough */

    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                      /* allocation failure – keep old */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Compute the n‑th term of A042032. */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)           /* already known */
        return cache[n];

    /* Remember the previous length before extending. */
    idx_t old_len = cache_len;

    /* Extend the cache to accommodate indices up to n. */
    ensure_cache_len(n);
    if (cache_len < old_len) cache_len = old_len;  /* realloc failed */

    /* Compute any missing terms using the recurrence. */
    for (idx_t i = old_len; i <= n; ++i)
    {
        /* For i < 16 the values are already in the base array. */
        if (i < 16) continue;

        cache[i] = cache[i - 8] * (seq_t)238142 - cache[i - 16];
    }

    return cache[n];
}
