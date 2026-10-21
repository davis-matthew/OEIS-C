/*
 * A090244  a(0) = 1; a(1) = 2; a(n) = { a(n-1) + a(n-2) for n even, a(n-1) -
 *          a(n-2) for n odd }.
 * https://oeis.org/A090244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1; a(1) = 2;
 * a(n) = (n even) ? a(n-1) + a(n-2) : a(n-1) - a(n-2)   for n >= 2
 */

#include <stdlib.h>

/* static storage for memoization */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;  /* number of elements stored, i.e. cache_len-1 is max index */
static idx_t cache_cap   = 0;  /* capacity of cache array */

/* ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already computed */

    /* grow capacity if needed */
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – behaviour undefined; exit is not allowed per spec */
        return;
    }
    cache = new_cache;
    cache_cap = new_cap;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else {
            if ((i & 1) == 0) {          /* even index */
                cache[i] = cache[i - 1] + cache[i - 2];
            } else {                     /* odd index */
                cache[i] = cache[i - 1] - cache[i - 2];
            }
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
