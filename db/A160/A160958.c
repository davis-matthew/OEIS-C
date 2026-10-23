/*
 * A160958  a(n) = (9^n - (-7)^n)/(9 - (-7)).
 * https://oeis.org/A160958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^n - (-7)^n)/(9 - (-7)) = (9^n - (-7)^n)/16 Recurrence:
 *            a(1) = 1, a(2) = 2, a(n) = 2*a(n-1) + 63*a(n-2), for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 1‑indexed cache for previously computed values. */
static seq_t *cache   = NULL;   /* cache[i] == a(i) for i ≥ 1 */
static idx_t cache_len = 0;     /* highest index computed so far */
static size_t cache_cap = 0;    /* allocated capacity (indices 1..cache_cap) */

/* Ensure the cache contains all values up to n. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {                 /* first allocation */
        cache_cap = (size_t)n;
        if (cache_cap < 2) cache_cap = 2;   /* need at least two base cases */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;               /* unused slot for 0‑index */
        cache_len = 0;
    } else if ((size_t)n > cache_cap) {   /* grow array if necessary */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            /* recurrence: a(n) = 2*a(n-1) + 63*a(n-2) */
            cache[i] = ((seq_t)2 * cache[i - 1]) + ((seq_t)63 * cache[i - 2]);
        }
    }
    cache_len = n;
}

/* Return the n-th term of sequence A160958. */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
