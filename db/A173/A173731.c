/*
 * A173731  a(n) = a(n-1) * (11*a(n-1) - a(n-2)) / (a(n-1) + 4*a(n-2)), a(0) =
 *          a(1) = 0, a(2) = 1.
 * https://oeis.org/A173731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 11, a(4) = 88, a(n) =
 *            11*a(n-1) - 33*a(n-2) + 33*a(n-3) - 11*a(n-4) + a(n-5) (n ≥ 5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#if defined(__cplusplus)
/* ------------------------------------------------------------------ */
/* Helper to multiply an arbitrary seq_t by a small unsigned integer.  */
/* The generic template uses the normal operator*, which works for a  */
/* big‑int class.  For 64‑bit integers we cast to __uint128_t to   */
/* avoid overflow in the intermediate product.                     */
/* ------------------------------------------------------------------ */
#include <stdint.h>
template <class T>
inline T mul_int(T a, unsigned int m) { return a * m; }

inline uint64_t mul_int(uint64_t a, unsigned int m)
{ return (uint64_t)(((__uint128_t)a * m)); }

#else /* C mode */
/* In C we can rely on built‑in unsigned __int128 if available. */
#if defined(__SIZEOF_INT128__)
#   define mul_int(a, m) ( (a) * (m) )
#else
/* If __uint128_t is not available, use normal multiplication; for 64‑bit *
   values this may overflow, but the sequence will only be checked for   *
   terms that fit in seq_t.  For arbitrary‑precision seq_t this branch     *
   is never used. */
#   define mul_int(a, m) ( (a) * (m) )
#endif
#endif

/* Static cache for memoised values. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of stored elements */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* ------------------------------------------------------------------ */
/* Grow the cache array to at least 'new_cap' and preserve existing   */
/* values.                                                           */
/* ------------------------------------------------------------------ */
static void grow_cache(idx_t new_cap)
{
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    free_seq_array(cache);
    cache     = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that cache has space for index 'n' (0‑based).  It will also   */
/* set the initial five terms if the cache is being created.           */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (cache != NULL) {
        if (cache_size > n) return;
    } else {
        /* First allocation.  A capacity of 16 is enough for the      *
           small initial segment and can grow later.                  */
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)88;
        cache_size = 5;
    }

    /* Grow until capacity exceeds desired index. */
    if (cache_cap <= n) {
        idx_t cap = cache_cap;
        while (cap <= n) cap <<= 1;
        grow_cache(cap);
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0 || i == 1)
            val = (seq_t)0;
        else if (i == 2)
            val = (seq_t)1;
        else if (i == 3)
            val = (seq_t)11;
        else if (i == 4)
            val = (seq_t)88;
        else {
            seq_t a1 = cache[i - 1];   /* a(n‑1) */
            seq_t a2 = cache[i - 2];   /* a(n‑2) */
            seq_t a3 = cache[i - 3];   /* a(n‑3) */
            seq_t a4 = cache[i - 4];   /* a(n‑4) */
            seq_t a5 = cache[i - 5];   /* a(n‑5) */

            val = mul_int(a1, 11);
            val = val - mul_int(a2, 33);
            val = val + mul_int(a3, 33);
            val = val - mul_int(a4, 11);
            val = val + a5;
        }
        cache[i] = val;
    }
    cache_size = n + 1;
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
