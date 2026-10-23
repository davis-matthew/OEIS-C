/*
 * A159330  Transform of the finite sequence (1, 0, -1, 0, 1) by the T_{1,1}
 *          transformation (see link).
 * https://oeis.org/A159330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n >= 7 Initial values:
 *            a(0)=2, a(1)=4, a(2)=9, a(3)=23, a(4)=55, a(5)=126, a(6)=292
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state used for memoisation */
static seq_t *cache   = NULL;   /* allocated array of seq_t values   */
static size_t cap    = 0;       /* number of elements allocated      */
static size_t filled = 0;       /* highest index + 1 that has been set */
static bool  init    = false;

/* grow the cache so that it can store at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = cap ? cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();          /* fatal error */
    cache = tmp;
    cap   = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* convert the 64‑bit index to a size_t (safe for n up to ~10^4) */
    size_t idx = (size_t)n;

    /* initialise the cache once */
    if (!init) {
        grow_cache(7);                       /* at least 7 entries */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)23;
        cache[4] = (seq_t)55;
        cache[5] = (seq_t)126;
        cache[6] = (seq_t)292;
        filled  = 7;
        init    = true;
    }

    /* ensure we can store the requested index */
    if (idx + 1 > cap)
        grow_cache(idx + 1);

    /* if the term is already known, return it immediately */
    if (idx < filled)
        return cache[idx];

    /* compute all missing terms up to `idx` using the recurrence */
    for (size_t i = filled; i <= idx; ++i) {
        cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
    }
    filled = idx + 1;
    return cache[idx];
}
