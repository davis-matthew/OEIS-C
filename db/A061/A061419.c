/*
 * A061419  a(n) = ceiling(a(n-1)*3/2) with a(1) = 1.
 * https://oeis.org/A061419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = ceil(a(n-1) * 3 / 2) (n ≥ 2)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent iterative form: a(n) = a(n-1) + ceil(a(n-1)/2) */
/* The function memoizes previously computed values so any order of calls works. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t   cache_len = 0;
static idx_t   cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset not used but safe */
    if (n <= cache_len) return seq_cache[n-1];

    /* enlarge cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out‑of‑memory */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base term if cache empty */
    if (cache_len == 0) {
        seq_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms */
    seq_t prev = seq_cache[cache_len-1];
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* next = prev + ceil(prev/2) */
        seq_t half = prev >> 1;          /* floor division by 2 */
        seq_t next = prev + half;
        if (prev & (seq_t)1) next += 1;  /* add 1 if remainder is 1 */
        seq_cache[i-1] = next;
        prev = next;
    }
    cache_len = n;
    return seq_cache[n-1];
}
