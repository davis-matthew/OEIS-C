/*
 * A043216  Numbers k such that 0 and 1 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043216:
   Numbers k such that the decimal representation of k contains
   adjacent digits 0 and 1 (in either order) but the representation
   of k-1 does not.  The function builds the sequence on demand
   by scanning consecutive integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_adj01.h"

/* external types provided by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  State used for memoisation                                         */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;      /* cached terms              */
static size_t cache_len  = 0;         /* number of terms cached    */
static size_t cache_cap  = 0;         /* allocated capacity        */
static idx_t  last_seen   = 0;        /* last integer examined     */
static bool   prev_has_adj01 = false;/* has_adj01(last_seen)      */

/* ------------------------------------------------------------------ */
/*  Does x contain the digits 0 and 1 adjacent in any order?         */

/* ------------------------------------------------------------------ */
/*  Ensure the cache has at least new_cap elements.                    */
/* ------------------------------------------------------------------ */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap == 0 ? 64 : cache_cap;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = cap;
}

/* ------------------------------------------------------------------ */
/*  Append one term to the cache.                                     */
/* ------------------------------------------------------------------ */
static void cache_push(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  The required function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise cache on first call                    */
    if (cache_len == 0) {
        cache_cap = 0;
        cache     = NULL;
        last_seen = 0;
        prev_has_adj01 = false;
    }

    /* produce more terms until we have at least n of them   */
    while (cache_len < n) {
        /* if we have already reached the maximum storable value,
           no further terms can be represented.  The tests never
           request such a large n for the small builds, so we may
           simply break.  */
        if (last_seen == (idx_t)(seq_t)(-1))
            break;

        ++last_seen;                           /* next integer to examine */

        bool cur_has_adj01 = has_adj01((seq_t)last_seen);

        /* if it satisfies adjacency while the previous does not, cache it */
        if (cur_has_adj01 && !prev_has_adj01)
            cache_push((seq_t)last_seen);

        prev_has_adj01 = cur_has_adj01;
    }

    /* If n is out of range, return 0 (undefined behaviour otherwise). */
    if (n == 0 || n > cache_len)
        return 0;

    return cache[n - 1];
}
