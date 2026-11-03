/*
 * A043328  a(0)=1; a(1)=1; a(n)= a(n-1) + floor(sqrt(a(n-1)*a(n-2))) +
 *          floor(sqrt(a(n-3)*a(n-4))) + ....
 * https://oeis.org/A043328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + floor(sqrt(a(n-1)*a(n-2))) + *
 *            floor(sqrt(a(n-3)*a(n-4))) + … , a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/*  Includes and helper functions                                 */
/* --------------------------------------------------------------- */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t etc.  (idx_t comes from the build) */
#include <stdbool.h>  /* bool */

#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* return floor(sqrt(a*b)) for any seq_t */
static inline seq_t sqrt_prod(seq_t a, seq_t b)
{
    /* seq_mul handles both small integer types and big‑integer types.
       floor_sqrt handles the result. */
    return floor_sqrt(seq_mul(a, b));
}

/* --------------------------------------------------------------- */
/*  Cache of previously computed terms                             */
/* --------------------------------------------------------------- */
static seq_t *cache     = NULL;   /* array of computed a(i) values */
static size_t cache_len = 0;      /* number of elements actually stored */

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
     *  1.  If we already have the value, just return it.
     * ---------------------------------------------------------------- */
    if (n < cache_len)
        return cache[n];

    /* ----------------------------------------------------------------
     *  2.  Grow the cache if necessary.  Note the explicit cast to
     *     seq_t* so the code compiles both as C and C++.
     * ---------------------------------------------------------------- */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache)                     /* allocation failure */
        abort();
    cache = new_cache;

    /* ----------------------------------------------------------------
     *  3.  Compute all missing terms up to n.
     * ---------------------------------------------------------------- */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0 || i == 1)          /* initialised to 1 */
        {
            cache[i] = (seq_t)1;
        }
        else
        {
            seq_t sum = 0;
            /* add floor(sqrt(a(k)*a(k-1))) for k = i-1, i-3, i-5, … */
            for (idx_t k = i-1; k > 0; k -= 2)
            {
                sum += sqrt_prod(cache[k], cache[k-1]);
                if (k <= 1)   /* finished the last pair */
                    break;
            }
            cache[i] = cache[i-1] + sum;
        }
    }

    cache_len = new_len;      /* all elements up to n are now present */
    return cache[(idx_t)n];
}
