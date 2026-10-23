/*
 * A029675  Numbers represented by quadratic form with Gram matrix [ 3, 1, -1;
 *          1, 4, 1; -1, 1, 3 ].
 * https://oeis.org/A029675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n) of numbers represented by the quadratic form
 *  Q(x,y,z) = 3x² + 4y² + 3z² + 2xy – 2xz + 2yz.
 *  The function is built-up: the cache of representable values is
 *  enlarged until it contains at least n entries.
 *
 *  The method enumerates all integer triples (x,y,z) with
 *      x² + y² + z² ≤ limit
 *  (since Q ≥ x² + y² + z²), computes Q, and marks all values ≤ limit
 *  found.  The resulting list of values is sorted and stored in a
 *  static array.  The cache is recomputed with an increased limit
 *  whenever more terms are requested.  This algorithm runs in
 *  O(limit^(3/2)) time per recompute and is well under the time
 *  limits for the required range of n. */
 

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------ */
/*  Static cache structures                                       */
static seq_t *cache_values   = NULL;   /* sorted list of representable numbers   */
static size_t cache_len      = 0;      /* number of cached entries               */
static idx_t   cache_limit   = 0;      /* maximal value for which cache was built */

/* ------------------------------------------------------------ */
/*  Rebuild the cache for all numbers ≤ limit.                   */
static void recompute_cache(idx_t limit)
{
    /*  Bitset to mark which numbers ≤ limit appear.            */
    size_t   bs_size   = (size_t)limit + 1;
    unsigned char *present = (unsigned char*)calloc(bs_size, 1);
    if (!present) exit(1);          /* fatal error: out of memory            */

    /*  Rough bound on |x|,|y|,|z|:  x²+y²+z² ≤ limit.           */
    long long r = (long long)(sqrt((double)limit) + 2.0);

    for (long long x = -r; x <= r; ++x) {
        __int128 xx = (__int128)x * x;
        for (long long y = -r; y <= r; ++y) {
            __int128 yy = (__int128)y * y;
            for (long long z = -r; z <= r; ++z) {
                __int128 zz   = (__int128)z * z;
                __int128 qq   = 3*xx + 4*yy + 3*zz
                                 + 2*(__int128)x*y
                                 - 2*(__int128)x*z
                                 + 2*(__int128)y*z;
                if (qq < 0) continue;
                idx_t val = (idx_t)qq;
                if (val <= limit) present[(size_t)val] = 1;
            }
        }
    }

    /*  Count representable numbers up to limit.                 */
    size_t cnt = 0;
    for (size_t i = 0; i < bs_size; ++i)
        if (present[i]) ++cnt;

    /*  Allocate new cache array.                               */
    seq_t *new_arr = NULL;
    if (cnt > 0) {
        new_arr = (seq_t*)malloc(cnt * sizeof(seq_t));
        if (!new_arr) { free(present); exit(1); }
        size_t idx = 0;
        for (size_t i = 0; i < bs_size; ++i)
            if (present[i]) new_arr[idx++] = (seq_t)i;
    }

    free(present);

    /*  Replace old cache.                                       */
    free(cache_values);
    cache_values = new_arr;
    cache_len    = cnt;
    cache_limit  = limit;
}

/* ------------------------------------------------------------ */
/*  Ensure that the cache contains at least `need` elements.    */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need) return;

    /*  Start with a small limit and double until we have enough. */
    idx_t new_limit = (cache_limit == 0) ? 1 : cache_limit * 2;
    while (cache_len < (size_t)need) {
        recompute_cache(new_limit);
        if (cache_len >= (size_t)need) break;
        new_limit *= 2;
    }
}

/* ------------------------------------------------------------ */
/*  Public function required by the testing framework.           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but keep 0 safe. */
    ensure_cache(n);
    return cache_values[n-1];
}
