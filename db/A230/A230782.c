/*
 * A230782  Number of n X 2 0..3 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 3*a(n-2) + 44*a(n-3) - 40*a(n-4) + 8*a(n-5) -
 *            192*a(n-6); Initial terms: a(1)=0, a(2)=6, a(3)=86, a(4)=820,
 *            a(5)=8154, a(6)=82576
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached values.  cache[0] is unused.  cache_n is the largest index that
   has a computed value. */
static seq_t *cache = NULL;
static idx_t cache_n = 0;

/* Return the n-th term of A230782.  Undefined for n < 1: returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* First call: allocate 7 elements (indices 0..6) and set base terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;          /* unused sentinel */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)86;
        cache[4] = (seq_t)820;
        cache[5] = (seq_t)8154;
        cache[6] = (seq_t)82576;
        cache_n = 6;
    }

    /* If the requested n is already cached, just return it. */
    if (n <= cache_n)
        return cache[n];

    /* Grow the cache to fit up to n.  Use realloc to avoid leaks. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;              /* reallocation failed */
    cache = tmp;

    /* Compute any new terms using the recurrence. */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        cache[i] = ((seq_t)10  * cache[i-1])
                 - ((seq_t)3   * cache[i-2])
                 + ((seq_t)44  * cache[i-3])
                 - ((seq_t)40  * cache[i-4])
                 + ((seq_t)8   * cache[i-5])
                 - ((seq_t)192 * cache[i-6]);
    }
    cache_n = n;
    return cache[n];
}
