/*
 * A227554  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having nonzero determinant, with
 *          rows and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 176, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227554: Number of n×2 binary arrays indicating 2×2 subblocks
   of some (n+1)×3 binary array with lexicographically nondecreasing
   rows/columns.
   Recurrence (valid for all n>6):
       a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
              - 15*a(n-4) + 6*a(n-5) - a(n-6)
   Initial terms:
       a(1)=4, a(2)=12, a(3)=33, a(4)=81, a(5)=179, a(6)=362.
   This implementation memoises the sequence so that calls may appear
   in any order.  The arithmetic uses only the operators available for
   the type seq_t, so it works with built‑in integer types, as well as
   with arbitrary‑precision classes.                                             */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised terms a(1), a(2), …   */
static size_t   cache_len = 0;  /* number of elements proved            */
static size_t   cache_cap = 0;  /* allocated capacity (in elements)     */

static const seq_t init[6] = {4, 12, 33, 81, 179, 362};

/* enlarge the cache so that it can hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();              /* out of memory – terminate */
    cache     = tmp;
    cache_cap = new_cap;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined index – harmless fallback */

    size_t target = (size_t)n;          /* need terms 0..n-1 */
    ensure_cache_cap(target);

    for (size_t k = cache_len; k < target; ++k)
    {
        if (k < 6)                       /* known initial values   */
            cache[k] = init[k];
        else
        {
            /* recurrence computation using only seq_t operators */
            seq_t term = 6 * cache[k-1] - 15 * cache[k-2]
                        + 20 * cache[k-3] - 15 * cache[k-4]
                        + 6 * cache[k-5] - cache[k-6];
            cache[k] = term;
        }
    }
    cache_len = target;
    return cache[n-1];
}
