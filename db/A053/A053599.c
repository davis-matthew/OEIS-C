/*
 * A053599  Number of nonempty subsequences {s(k)} of 1..n such that the
 *          difference sequence is palindromic.
 * https://oeis.org/A053599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(n)=2*a(n-2)+2*n-1 for n>2
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n>=1.  Uses memoisation so calls in any order are fast. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array holding computed terms */
static size_t cache_cap = 0;         /* allocated capacity (size of cache array) */
static size_t cache_max = 0;         /* largest n that has been computed */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache_cap(idx_t n) {
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* memory error – caller will get 0 */
    cache = tmp;
    cache_cap = new_cap;
    /* initialise new cells to 0 (not strictly needed because we overwrite them) */
    for (size_t i = cache_max + 1; i < cache_cap; ++i)
        cache[i] = 0;
}

/* Compute missing terms up to n, using the recurrence */
static void compute_upto(idx_t n) {
    if (n <= cache_max) return;      /* already computed */

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)3;
        } else {
            cache[i] = (seq_t)2 * cache[i - 2] + (seq_t)2 * i - (seq_t)1;
        }
    }
    cache_max = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1, behaviour below offset is irrelevant */

    ensure_cache_cap(n);
    compute_upto(n);
    return cache[n];
}
