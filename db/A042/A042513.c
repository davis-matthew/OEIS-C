/*
 * A042513  Denominators of continued fraction convergents to sqrt(785).
 * https://oeis.org/A042513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 56*a(n-1) + a(n-2), a(0)=1, a(1)=56
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the terms of OEIS sequence A042513 using a
 * simple recurrence with memoisation.  It is written in a way that
 * compiles in both C11 and C++17.  No assumptions are made about the
 * order in which a() is called. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;  /* number of terms stored */
static idx_t  cache_cap = 0;  /* capacity of the array */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= n) {                    /* need more space  */
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;   /* double until > n   */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();              /* allocation failed   */
        cache   = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)56;

    /* Initialise cache on first use */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)56;
        cache_len = 2;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_len) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_capacity(i);
        cache[i] = (seq_t)56 * cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
