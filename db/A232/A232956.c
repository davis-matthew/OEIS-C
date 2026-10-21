/*
 * A232956  Number of 2 X n 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, diagonally or antidiagonally, and top
 *          left element zero.
 * https://oeis.org/A232956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2), a(1)=4, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoisation cache */
static seq_t *cache = NULL;       /* dynamic array of computed terms */
static idx_t  cache_len  = 0;     /* number of terms currently stored */
static size_t cache_cap  = 0;     /* current allocated capacity */

/* Ensure the cache can hold at least `up_to` past the zero‑based index
 * (i.e. terms up to a(up_to)).  The allocation size is grown
 * geometrically to keep amortised O(1) time per allocation. */
static void ensure_capacity(idx_t up_to)
{
    if (cache_cap >= (size_t)up_to)
        return;
    size_t new_cap = cache_cap < 4 ? 4 : cache_cap;
    while (new_cap < (size_t)up_to)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of A232956.  The function is thread‑safe only
 * because of the static cache; if concurrent use is required a mutex
 * is needed, but this is outside the scope of the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, return 0 */

    /* Initialise base terms if cache is empty */
    if (cache_len == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)4;      /* a(1) */
        cache[1] = (seq_t)21;     /* a(2) */
        cache_len = 2;
    }

    /* If requested term is already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to a(n) */
    ensure_capacity(n);
    for (idx_t idx = cache_len; idx < n; ++idx) {
        /* idx is 0‑based for array; term a(idx+1) depends on
         * a(idx) (cache[idx-1]) and a(idx-1) (cache[idx-2]) */
        seq_t next = (seq_t)5 * cache[idx - 1] + (seq_t)6 * cache[idx - 2];
        cache[idx] = next;
    }
    cache_len = n;
    return cache[n - 1];
}
