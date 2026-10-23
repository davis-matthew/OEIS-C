/*
 * A104720  Expansion of 1/((1-x)(1-x^2)(1-10x)).
 * https://oeis.org/A104720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + (2*n+3+(-1)^n)/4, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a memoized iterative recurrence.  Works for any seq_t defined
   by the build, whether it is a native integer type or a big‑int
   class. */

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#  include <new>
#endif

static seq_t *cache = NULL;         /* array of computed terms   */
static idx_t cache_cap = 0;         /* current allocated size    */
static idx_t cache_size = 0;        /* number of terms stored    */

/* Ensure that cache[0..needed] exists, allocating/reallocating if
   necessary.  The first term a(0) = 1 is initialized lazily. */
static void ensure_cache(idx_t needed)
{
    /* we need indices 0 .. needed, i.e. (needed+1) slots */
    if (needed + 1 <= cache_cap)
        return;

    /* new capacity: at least double the needed index plus a small
       constant to avoid frequent reallocations. */
    idx_t new_cap = needed * 2 + 10;
    if (new_cap < needed + 1)
        new_cap = needed + 1;

    seq_t *newp;
#ifdef __cplusplus
    newp = new seq_t[new_cap];
    /* copy old values */
    for (idx_t i = 0; i < cache_size; ++i)
        newp[i] = cache[i];
    /* delete old block */
    if (cache)
        delete[] cache;
#else
    newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = newp;
    cache_cap = new_cap;

    /* initialise a(0) if this is the very first allocation */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_size = 1;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* make sure we have at least n+1 cached values */
    ensure_cache(n);

    if (n < cache_size)
        return cache[n];

    /* compute missing terms sequentially */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* term = (2*i + 3 + (-1)^i) / 4
           which simplifies to:
           if i even  -> (i + 2) / 2
           if i odd   -> (i + 1) / 2 */
        seq_t term = (seq_t)((i + ((i & 1ULL) == 0 ? 2 : 1)) / 2);

        cache[i] = cache[i - 1] * (seq_t)10 + term;
    }
    cache_size = n + 1;
    return cache[n];
}
