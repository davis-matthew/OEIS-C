/*
 * A010076  a(n) = sum of base-9 digits of a(n-1) + sum of base-9 digits of
 *          a(n-2).
 * https://oeis.org/A010076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of base-9 digits of a(n-1) + sum of base-9 digits of
 *            a(n-2) a(0)=0, a(1)=1, a(2)=1;
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
    /* In C++ we need a reinterpret_cast because realloc returns void* */
    #define RECAST_PTR(p) reinterpret_cast<seq_t*>(p)
#else
    /* In C a void* can be implicitly converted to any object type */
    #define RECAST_PTR(p) ((seq_t*)p)
#endif

/* Static cache for already computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ------------------------------------------------------------ */
/* Returns the sum of the base‑9 digits of v.                 */
/* Works for both integer types and the arbitrary‑precision    */
/* integer class because % and / are defined for that class.   */
static inline seq_t digit_sum_base9(seq_t v)
{
    seq_t sum = (seq_t)0;
    while (v > (seq_t)0)
    {
        sum += v % (seq_t)9;
        v   /= (seq_t)9;
    }
    return sum;
}

/* ------------------------------------------------------------ */
/* Makes sure that the cache contains indices 0 … upto.         */
/* Reallocates if necessary, initializes the first three terms, */
/* and computes any new ones using the recurrence.             */
static void ensure_cache(idx_t upto)
{
    if (cache_size > upto)
        return;

    /* Grow the array to hold indices 0 … upto */
    idx_t const new_size = upto + (idx_t)1;
    seq_t *p = RECAST_PTR(realloc(cache, new_size * sizeof(seq_t)));
    if (!p)
        exit(1);           /* allocation failed – abort */
    cache = p;

    /* Initialise the first three known terms (over‑write if already set) */
    if (new_size > 0)
        cache[0] = (seq_t)0;
    if (new_size > 1)
        cache[1] = (seq_t)1;
    if (new_size > 2)
        cache[2] = (seq_t)1;

    /* Compute any new terms beyond the previous cache_size          */
    idx_t const start = (cache_size < (idx_t)3) ? (idx_t)3 : cache_size;
    for (idx_t i = start; i <= upto; ++i)
    {
        seq_t s1 = digit_sum_base9(cache[i - (idx_t)1]);
        seq_t s2 = digit_sum_base9(cache[i - (idx_t)2]);
        cache[i] = s1 + s2;
    }

    cache_size = new_size;
}

/* ------------------------------------------------------------ */
/* Public function required by the build.                      */
/* Returns a(n) for any n >= 0.                                */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
