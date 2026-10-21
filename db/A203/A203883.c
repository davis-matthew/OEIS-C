/*
 * A203883  Number of (n+1)X5 0..6 arrays with every 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A203883 computed via a 7‑term linear recurrence * a(n) =
 *            115*a(n-1) +17016*a(n-2) -763308*a(n-3) -1177416*a(n-4) *
 *            +49714560*a(n-5) +35870688*a(n-6) -153031680*a(n-7) * Initial
 *            values: a(1)=1399489, a(2)=222645367, a(3)=39572771665 *
 *            a(4)=7265109130279, a(5)=1337957259758497 *
 *            a(6)=24709006959595...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

/* static cache for computed terms.  cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored (≥0) */

/* Ensure the cache has at least 'need' elements (need ≥ current size).  */
static void grow_cache(idx_t need)
{
    if (cache_size >= need)
        return;
    idx_t new_cap = (need > cache_size * 2) ? need : cache_size * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();    /* allocation failed – the caller cannot recover safely */
    cache = new_cache;
    cache_size = new_cap;
}

/* Initialize terms a(1) … a(7).  */
static void init_cache(void)
{
    /* allocate enough space for the first 7 terms */
    grow_cache(7);

    cache[0] = 1399489ULL;                      /* a(1) */
    cache[1] = 222645367ULL;                   /* a(2) */
    cache[2] = 39572771665ULL;                 /* a(3) */
    cache[3] = 7265109130279ULL;               /* a(4) */
    cache[4] = 1337957259758497ULL;            /* a(5) */
    cache[5] = 247090069595954263ULL;          /* a(6) */
    cache[6] = seq_from_str("45601045546133880817");  /* a(7) */
}

/* Compute a(n) for n ≥ 1, using the recurrence and possibly extending the cache. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for n < 1; caller should not request it */

    if (cache == NULL)
        init_cache();

    /* convert n to 0‑based index */
    idx_t idx = n - 1;

    /* Grow the cache if necessary */
    if (idx >= cache_size)
        grow_cache(idx + 1);

    /* If the value is already known, return it */
    /* (cache elements beyond what we have computed yet are zero-initialised
     * by realloc since we only fill entries when we compute them.) */
    if (idx < 7)
        return cache[idx];

    /* Compute missing terms up to idx using the 7‑term recurrence */
    for (idx_t k = 7; k <= idx; ++k)
    {
        /* a(k+1) is stored in cache[k] (since cache[0] = a(1)) */
        seq_t val = 0;
        val += (seq_t)115        * cache[k-1];
        val += (seq_t)17016      * cache[k-2];
        val -= (seq_t)763308     * cache[k-3];
        val -= (seq_t)1177416    * cache[k-4];
        val += (seq_t)49714560   * cache[k-5];
        val += (seq_t)35870688   * cache[k-6];
        val -= (seq_t)153031680  * cache[k-7];
        cache[k] = val;
    }

    return cache[idx];
}
