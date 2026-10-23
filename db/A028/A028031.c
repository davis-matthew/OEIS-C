/*
 * A028031  Expansion of 1/((1-3x)(1-4x)(1-5x)(1-12x)).
 * https://oeis.org/A028031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A028031: Expansion of 1/((1-3x)(1-4x)(1-5x)(1-12x))
 * Recurrence: a(0)=1, a(1)=24, a(2)=385, a(3)=5280,
 *             a(n)=24*a(n-1)-191*a(n-2)+624*a(n-3)-720*a(n-4)  (n≥4)
 *
 * All operations are performed with 128‑bit intermediates (when
 * available) to avoid overflow for the 64‑bit builds.  The routine
 * memoises computed terms so that calls can be made in any order.
 */

#include <stdlib.h>
#if defined(__SIZEOF_INT128__)
// 128‑bit integer type is available.
typedef __int128  int128_t;
typedef unsigned __int128 uint128_t;
#else
// Fallback – use 64‑bit arithmetic.
typedef long long  int128_t;
typedef unsigned long long uint128_t;
#endif

/* ------------------------------------------------------------------ */
/* static helper data                                                      */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;    /* array of computed a(n) */
static size_t capacity = 0;      /* allocated array size      */
static size_t size     = 0;      /* number of terms stored    */

/* ensure the cache can hold at least need elements */
static void ensure_capacity(size_t need)
{
    if (need <= capacity) return;
    size_t new_cap = (capacity == 0) ? 16 : capacity * 2;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                     /* allocation failure – keep old array */
    cache = tmp;
    capacity = new_cap;
}

/* ------------------------------------------------------------------ */
/* required exported function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache)
    {
        ensure_capacity(16);
        size = 0;
    }

    /* compute missing terms up to n */
    if (n >= (idx_t)size)
    {
        ensure_capacity((size_t)n + 1);
        for (idx_t i = (idx_t)size; i <= n; ++i)
        {
            if (i == 0)            cache[0] = (seq_t)1;
            else if (i == 1)       cache[1] = (seq_t)24;
            else if (i == 2)       cache[2] = (seq_t)385;
            else if (i == 3)       cache[3] = (seq_t)5280;
            else
            {
                int128_t term = 0;
                term += (int128_t)cache[i-1] * 24;
                term -= (int128_t)cache[i-2] * 191;
                term += (int128_t)cache[i-3] * 624;
                term -= (int128_t)cache[i-4] * 720;
                cache[i] = (seq_t)term;
            }
        }
        size = (size_t)n + 1;
    }

    return cache[n];
}
