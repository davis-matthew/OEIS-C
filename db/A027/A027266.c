/*
 * A027266  a(n) = Sum_{k=0..2n} (k+1) * A026519(n, k).
 * https://oeis.org/A027266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027266: a(n) = Sum_{k=0..2n} (k+1) * A026519(n,k)
   Recurrence: a(0)=1, a(1)=6, a(2)=18, a(3)=72,
                a(n)=12*a(n-2)-36*a(n-4)   (n≥4) */
#include <stdlib.h>
#include <stddef.h>

/* Internal state for memoisation */
static seq_t *a_cache = NULL;          /* array of computed terms          */
static size_t a_cache_cap = 0;         /* allocated capacity                */
static size_t a_cache_len = 0;         /* number of terms that have been
                                            computed (valid indices 0..len-1) */

/* Ensure that the cache is allocated and large enough for indices
   0 .. needed-1. New elements are left uninitialised.           */
static void a_ensure_capacity(size_t needed)
{
    if (needed <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* Initialise the cache with the four seed values. */
static void a_init(void)
{
    a_ensure_capacity(8);           /* a bit of slack for future growth */
    a_cache[0] = 1;
    a_cache[1] = 6;
    a_cache[2] = 18;
    a_cache[3] = 72;
    a_cache_len = 4;
}

/* Public entry point.  The function is safe for repeated calls in
   any order and provides memoisation for speed.                */
seq_t a(idx_t n)
{
    if (!a_cache) a_init();                /* first call */

    if ((idx_t)n < (idx_t)a_cache_len) {
        return a_cache[n];
    }

    /* Need to compute up to n                                   */
    size_t needed = (size_t)n + 1;
    if (needed > a_cache_cap) {
        a_ensure_capacity(needed);
    }

    /* Extend the cached sequence */
    for (idx_t i = (idx_t)a_cache_len; i <= n; ++i) {
        a_cache[i] = ((seq_t)12) * a_cache[i - 2] -
                     ((seq_t)36) * a_cache[i - 4];
    }
    a_cache_len = needed;

    return a_cache[n];
}
