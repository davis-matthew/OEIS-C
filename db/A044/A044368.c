/*
 * A044368  Numbers n such that string 3,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044368:  numbers n such that the decimal representation of n contains
   the substring "36" while n-1 does not.  The function searches for the
   required numbers by scanning integers in increasing order and caching
   the results. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x contains the consecutive digits 3 and 6. */
static bool has36(seq_t x)
{
    if (x < 10)          /* a single digit can never contain "36" */
        return false;

    seq_t prev = x % 10; /* last (least‑significant) digit */
    x     /= 10;
    while (x >= 1)
    {
        seq_t cur = x % 10;
        if (cur == 3 && prev == 6)      /* found "36" */
            return true;
        prev = cur;
        x   /= 10;
    }
    return false;
}

/* Ensure *cache has room for at least needed elements. */
static void grow_cache(size_t needed, seq_t **cache, size_t *cap)
{
    size_t new_cap = (*cap > 0 ? *cap << 1 : 8);
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory – cannot recover */

    *cache = tmp;
    *cap   = new_cap;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Cached terms: cache[0] = a(1) */
    static seq_t       *cache      = NULL;
    static idx_t        cache_size = 0;   /* number of terms stored      */
    static size_t       cache_cap  = 0;   /* allocated capacity          */
    static seq_t        last_checked = 0; /* last integer examined       */

    if (n <= 0)
        return (seq_t)0;                  /* undefined for n < 1 */

    /* Return cached term if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_size < n)
    {
        ++last_checked;                             /* examine next integer */
        bool cur_has  = has36(last_checked);
        bool prev_has = false;
        if (last_checked > 0)
            prev_has = has36(last_checked - 1);

        if (cur_has && !prev_has)
        {
            if (cache_size >= cache_cap)
                grow_cache(cache_size + 1, &cache, &cache_cap);

            cache[cache_size++] = last_checked;
        }
    }

    return cache[n - 1];
}
