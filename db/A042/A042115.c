/*
 * A042115  Denominators of continued fraction convergents to sqrt(582).
 * https://oeis.org/A042115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 386*a(n-2) - a(n-4) for n>3 Initial terms: a(0)=1,
 *            a(1)=8, a(2)=385, a(3)=3088
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the n-th term of the sequence A042115.
   It uses a static cache to store previously computed values, so that
   calls in any order are efficient.  */
#include <stddef.h>
#if !defined(__cplusplus)
#include <stdlib.h>
#endif

#if defined(__cplusplus)
# include <vector>
#endif

static
#if defined(__cplusplus)
    std::vector<seq_t> cache_vec;
#else
    seq_t *cache_ptr = NULL;
    size_t cache_cap = 0;
#endif

/* Ensure that the cache contains values up to index n. */
static void
ensure_up_to(idx_t n)
{
#if defined(__cplusplus)
    const size_t need = (size_t)n + 1;
    const size_t old = cache_vec.size();
    if (need <= old) return;

    cache_vec.resize(need);

    /* Set base values if not already set. */
    if (old <= 0) {
        if (need > 0) cache_vec[0] = (seq_t)1;
        if (need > 1) cache_vec[1] = (seq_t)8;
        if (need > 2) cache_vec[2] = (seq_t)385;
        if (need > 3) cache_vec[3] = (seq_t)3088;
    } else {
        /* Old cache may contain some of the base values or not.
           Ensure they are set correctly. */
        if (old <= 0 && need > 0) cache_vec[0] = (seq_t)1;
        if (old <= 1 && need > 1) cache_vec[1] = (seq_t)8;
        if (old <= 2 && need > 2) cache_vec[2] = (seq_t)385;
        if (old <= 3 && need > 3) cache_vec[3] = (seq_t)3088;
    }

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = (idx_t)old; i <= n; ++i) {
        if (i < 4) continue;               /* base values already set */
        cache_vec[(size_t)i] = (seq_t)386 * cache_vec[(size_t)(i-2)]
                               - cache_vec[(size_t)(i-4)];
    }

#else
    const size_t need = (size_t)n + 1;
    if (need <= cache_cap) return;

    /* Allocate (or grow) the cache.  realloc preserves old contents. */
    seq_t *tmp = (seq_t *)realloc(cache_ptr, need * sizeof(seq_t));
    if (!tmp) { /* allocation failed – abort or handle as needed */ }
    cache_ptr = tmp;
    cache_cap = need;

    /* Set base values if not already present. */
    if (n >= 0) cache_ptr[0] = (seq_t)1;
    if (n >= 1) cache_ptr[1] = (seq_t)8;
    if (n >= 2) cache_ptr[2] = (seq_t)385;
    if (n >= 3) cache_ptr[3] = (seq_t)3088;

    /* Compute remaining terms. */
    for (idx_t i = 4; i <= n; ++i) {
        cache_ptr[(size_t)i] = (seq_t)386 * cache_ptr[(size_t)(i-2)]
                               - cache_ptr[(size_t)(i-4)];
    }
#endif
}

/* External function requested by the build system. */
seq_t
a(idx_t n)
{
    ensure_up_to(n);
#if defined(__cplusplus)
    return cache_vec[(size_t)n];
#else
    return cache_ptr[(size_t)n];
#endif
}
