/*
 * A151574  a(0)=1, a(1)=2; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; for n ≥ 2: let i = ⌊log₂ n⌋, p = 2ⁱ, j = n – p;
 *            a(n) = a(j) + 2·a(j+1)
 * Range:     u8 n <= 58, u16 n <= 2038, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *a_cache = NULL;   /* static cache of computed values */
static size_t a_cache_cap = 0;  /* current capacity of the cache */

/* Ensure the cache can hold values up to index 'n' */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_cap) return;
    size_t new_cap = (n + 1 > a_cache_cap * 2) ? (size_t)(n + 1)
                                               : (a_cache_cap ? a_cache_cap * 2 : 1);
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* OOM – terminate */
    if (tmp != a_cache) a_cache = tmp;
    /* initialise newly allocated space to 0 (unused values) */
    for (size_t i = a_cache_cap; i < new_cap; ++i) a_cache[i] = 0;
    a_cache_cap = new_cap;
}

/* Recursive computation with memoisation */
static seq_t a_rec(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* value may already be cached */
    if (a_cache[n]) return a_cache[n];

    /* find highest power of two ≤ n */
    idx_t p = 1;
    while (p <= n && (p << 1) <= n) p <<= 1;

    idx_t j = n - p;
    seq_t val = a_rec(j) + (a_rec(j + 1) << 1);
    a_cache[n] = val;
    return val;
}

/* public entry point required by the harness */
extern seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_rec(n);
}
