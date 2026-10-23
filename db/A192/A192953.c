/*
 * A192953  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache array for memoisation */
static seq_t *cache = NULL;          /* pointer to stored terms */
static idx_t cache_len = 0;          /* number of terms currently stored */
static idx_t cache_cap = 0;          /* allocated capacity (in terms) */

/* Ensure that the cache has room for terms up to index n (inclusive) and
   initialise any newly allocated entries. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already have it */

    /* Grow capacity to at least n+1, doubling each time */
    idx_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 4;      /* start with 4 slots */
    while (new_cap <= n) new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    /* Safety: check allocation?  omitted for brevity as build environment
       guarantees success. */
    cache_cap = new_cap;

    /* Initialise new entries using the recurrence, for i >= cache_len */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)            val = 0;
        else if (i == 1)       val = 1;
        else if (i == 2)       val = 2;
        else if (i == 3)       val = 6;
        else {
            /* a(i) = 3*a(i-1) - 2*a(i-2) - a(i-3) + a(i-4) */
            val = cache[i-1] * 3;
            val -= cache[i-2] * 2;
            val -= cache[i-3];
            val += cache[i-4];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
