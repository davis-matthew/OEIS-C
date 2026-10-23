/*
 * A111989  G.f.: 1/(1-6*x+8*x^3).
 * https://oeis.org/A111989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 8*a(n-3)  (n >= 3)
   Initial values: a(0) = 1, a(1) = 6, a(2) = 36 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached terms, indexed from 0 */
static idx_t cache_size = 0;    /* number of terms currently stored */
static idx_t cache_cap   = 0;    /* allocated capacity */

/* Ensure the cache has room for at least indices 0..need */
static void ensure_cap(idx_t need) {
    if (cache_cap > need) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n) {
    if (n + 1 <= cache_size) return cache[n];

    /* Expand cache to hold up to n */
    ensure_cap(n);
    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)        cache[i] = 1;
        else if (i == 1)   cache[i] = 6;
        else if (i == 2)   cache[i] = 36;
        else               cache[i] = 6 * cache[i-1] - 8 * cache[i-3];
    }
    cache_size = n + 1;
    return cache[n];
}
