/*
 * A100132  a(n) = Sum_{k=0..floor(n/4)} C(n-2k,2k) * 2^(n-3k).
 * https://oeis.org/A100132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} C(n-2k, 2k) * 2^(n-3k) Recurrence
 *            (valid for n >= 4): a(0)=1, a(1)=2, a(2)=4, a(3)=8 a(n) =
 *            4*a(n-1) - 4*a(n-2) + 2*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses an on‑demand memoised iterative
   computation based on the recurrence.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of already computed values     */
static size_t   cache_len = 0;       /* number of values stored (>=1 if init)*/
static size_t   cache_cap = 0;       /* current allocated capacity           */
static bool     initialized = false; /* has initial values been stored?      */

/* Allocate (or reallocate) cache to hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap <<= 1; /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate program (must be safe in runner). */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with first four terms. */
static void init_cache(void)
{
    ensure_cache_size(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)8;
    cache_len = 4;
    initialized = true;
}

/* Compute all terms up to 'n' (inclusive). Assumes n is small enough to fit
   in size_t and that cache has been initialised. */
static void compute_up_to(idx_t n)
{
    if (!initialized)
        init_cache();

    /* If n already computed, nothing to do. */
    if ((size_t)n < cache_len)
        return;

    /* Extend cache to accommodate 'n'. */
    ensure_cache_size((size_t)n + 1);

    /* Compute missing values iteratively using recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = 4*a(i-1) - 4*a(i-2) + 2*a(i-4)   */
        seq_t t1 = cache[i-1] * (seq_t)4;   /* 4 * a(i-1) */
        seq_t t2 = cache[i-2] * (seq_t)4;   /* 4 * a(i-2) */
        seq_t t3 = cache[i-4] * (seq_t)2;   /* 2 * a(i-4) */
        cache[i] = t1 - t2 + t3;
    }

    cache_len = (size_t)n + 1;
}

/* Public entry point.  */
seq_t a(idx_t n)
{
    /* Guard against exceedingly large n that would overflow size_t. */
    if ((size_t)n > SIZE_MAX) {
        /* Cannot compute – return zero (indicates overflow). */
        return (seq_t)0;
    }

    compute_up_to(n);
    return cache[(size_t)n];
}
