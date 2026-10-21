/*
 * A085376  Ratio-dependent insertion sequence I(0.36704) (see the link
 *          below).
 * https://oeis.org/A085376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------
  Sequence: OEIS A085376
  Ratio‑dependent insertion sequence I(0.36704)
  
  Recurrence (conjectured):  a(n) = 10*a(n-2) - a(n-4)
  Initial values:  a(1)=1, a(2)=3, a(3)=11, a(4)=30
--------------------------------------------------------------------*/
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/*--------------------------------------------------------------------
  Compute the term a(n) for n >= 1.
  Uses memoisation; any order of calls is supported.
--------------------------------------------------------------------*/

#ifdef __cplusplus
/* C++ implementation – use std::vector for dynamic storage */
static std::vector<seq_t> cache;   /* indices 0..n, element 0 unused */
static idx_t cached = 0;           /* largest index with computed value */

/* Ensure cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;

    /* we always need at least indices 0..4 available */
    size_t needed = (size_t)((n > 4 ? n : 4) + 1);
    if (cache.size() < needed)
        cache.resize(needed);      /* default‑construction gives zeros */

    /* initialise missing initial terms */
    if (cached < 1) cache[1] = (seq_t)1;
    if (cached < 2) cache[2] = (seq_t)3;
    if (cached < 3) cache[3] = (seq_t)11;
    if (cached < 4) cache[4] = (seq_t)30;

    /* compute new terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i)
    {
        if (i <= 4)          /* initial values already set */
            continue;
        cache[i] = (seq_t)10 * cache[i-2] - cache[i-4];
    }
    cached = n;
}

/* Public sequence function (external linkage) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n<1, placeholder */
    ensure_cache(n);
    return cache[n];
}

#else
/* C implementation – use malloc/realloc for dynamic array */
static seq_t *cache = NULL;   /* array index 0 unused */
static idx_t cached = 0;      /* max index that has valid value */

/* Ensure cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cached)
        return;

    /* Ensure indices 0..4 are present */
    size_t needed = (size_t)((n > 4 ? n : 4) + 1);
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
    {
        /* Allocation failed – abort (no partial changes) */
        abort();
    }
    cache = tmp;

    /* Initialise missing initial terms */
    if (cached < 1) cache[1] = (seq_t)1;
    if (cached < 2) cache[2] = (seq_t)3;
    if (cached < 3) cache[3] = (seq_t)11;
    if (cached < 4) cache[4] = (seq_t)30;

    /* Compute new terms */
    for (idx_t i = cached + 1; i <= n; ++i)
    {
        if (i <= 4)
            continue;
        cache[i] = (seq_t)10 * cache[i-2] - cache[i-4];
    }
    cached = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n<1 */
    ensure_cache(n);
    return cache[n];
}
#endif
