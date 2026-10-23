/*
 * A117475  The values of c in a^2 + b^2 = c^2 where b - a = 23 and gcd(a,b,c)
 *          = 1.
 * https://oeis.org/A117475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117475
 *  a(1) = 37
 *  a(2) = 65
 *  a(3) = 205
 *  a(4) = 373
 *  a(n) = 6*a(n-2) - a(n-4)   for n ≥ 5
 *
 * The implementation memoises previously computed terms in a static
 * dynamic array.  It works with any unsigned integer type or an
 * arbitrary‑precision integer class that supports +, –, * and
 * C‑style casts.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of already computed values   */
static size_t  cache_size = 0;/* number of valid entries in cache   */
static size_t  cache_cap   = 0;/* current array capacity              */

/* Helper: compute the recurrence using a temporary variable.
 * The expression is valid for both built‑in unsigned types and
 * arbitrary‑precision types that support '+', '-' and '*'.
 */
static inline seq_t calc_recurrence(seq_t a_nm2, seq_t a_nm4)
{
    seq_t tmp = (seq_t)6 * a_nm2;   /* 6 * a(n-2)                     */
    tmp -= a_nm4;                   /* subtract a(n-4)                 */
    return tmp;
}

/* Ensure that the cache contains at least n terms (1‑based index). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_size)
        return;                       /* nothing to do                   */

    /* Grow the array if required. */
    size_t needed = (size_t)n;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                   /* allocation failed                */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four values if the cache is empty. */
    if (cache_size == 0) {
        cache[0] = 37;
        cache[1] = 65;
        cache[2] = 205;
        cache[3] = 373;
        cache_size = 4;
    }

    /* Compute missing terms up to the required index.            */
    while (cache_size < needed) {
        size_t i = cache_size;            /* zero‑based index for a(i+1)   */
        size_t idx_nm2 = i - 2;            /* a(i-1) → a(n-2)              */
        size_t idx_nm4 = i - 4;            /* a(i-3) → a(n-4)              */
        cache[i] = calc_recurrence(cache[idx_nm2], cache[idx_nm4]);
        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for non‑positive n   */
    ensure_cache_size(n);
    return cache[n - 1];
}
