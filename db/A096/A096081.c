/*
 * A096081  a(n) = floor(a(n-2)^2/a(n-1)) + a(n-1) + a(n-2), a(0) = 0, a(1) =
 *          1, a(2) = 1, ...
 * https://oeis.org/A096081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-2)^2 / a(n-1)) + a(n-1) + a(n-2), with a(0) =
 *            0, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 21, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A096081.
   The function is memoized so that calls may come in any order.
   It works for unsigned 8,16,32,64 or 128 bit types as well as
   arbitrary‑precision arithmetic classes (operator overloaded).
*/
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;      /* memoized sequence */
static size_t  cache_size = 0;   /* next index to be filled: cache[0..cache_size-1] are valid */
static size_t  cache_cap   = 0;   /* allocated capacity */

/* Ensure that cache holds values up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial capacity of at least 3 to hold a(0)..a(2) */
        cache_cap = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1); /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_size = 3;
    }

    /* Extend cache capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap * 2 ? new_cap * 2 : 4; /* grow at least 2x or to 4 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t prev2  = cache[i - 2];
        seq_t part   =(prev2 * prev2) / prev;   /* floor division */
        cache[i] = part + prev + prev2;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;          /* base values */

    ensure_cache(n);     /* computes up to n if necessary */
    return cache[n];
}
