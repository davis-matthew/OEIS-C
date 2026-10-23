/*
 * A130241  Maximal index k of a Lucas number such that Lucas(k) <= n (the
 *          'lower' Lucas (A000032) Inverse).
 * https://oeis.org/A130241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 309, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the largest k so that Lucas(k) <= n
   (the inverse of the Lucas sequence).  The function expands a
   static cache of Lucas numbers on demand and binary‑searches it.
*/

#include <stdlib.h>

static seq_t *lucas_cache = NULL;   /* 1‑based array of Lucas numbers */
static idx_t lucas_len   = 0;        /* number of stored numbers          */
static idx_t lucas_cap   = 0;        /* allocated capacity                 */

/* ensure lucas_cache has capacity for at least 'needed' entries (1‑based) */
static void ensure_lucas_cap(idx_t needed)
{
    if (needed < lucas_cap) return;
    size_t newcap = (lucas_cap > 0) ? lucas_cap * 2 : 4;
    while (newcap <= needed) newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(lucas_cache, newcap * sizeof(seq_t));
    /* In C++ builds which use a class type for seq_t,
       the objects are default‑constructed by this allocation. */
    if (newptr) {
        lucas_cache = newptr;
        lucas_cap   = newcap;
    }
}

/* initialise the cache with the first two Lucas numbers */
static void init_lucas(void)
{
    ensure_lucas_cap(2);
    lucas_cache[1] = (seq_t)1;   /* L(1) */
    lucas_cache[2] = (seq_t)3;   /* L(2) */
    lucas_len     = 2;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_lucas();
        initialized = 1;
    }

    seq_t limit = (seq_t)n;

    /* expand cache until the next Lucas number would exceed 'limit' */
    while (1) {
        idx_t i = lucas_len;                /* last stored index */
        if (i < 2) break;                   /* safety, should not happen */
        seq_t a_val = lucas_cache[i];       /* L(i)   */
        seq_t b_val = lucas_cache[i-1];     /* L(i-1) */

        /* check for overflow or exceeding limit: a > limit - b */
        if (a_val > limit - b_val) break;   /* cannot add safely or would be too large */

        seq_t c_val = a_val + b_val;        /* L(i+1) */
        if (c_val > limit) break;           /* safety, already handled above */

        ensure_lucas_cap(i + 1);
        lucas_len++;
        lucas_cache[lucas_len] = c_val;
    }

    /* binary search for largest index k with L(k) <= limit */
    idx_t low  = 1;
    idx_t high = lucas_len;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (lucas_cache[mid] <= limit)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
