/*
 * A129380  Partial sums of A129379.
 * https://oeis.org/A129380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A129380: a(n) = (1/2)*(n^2 - 5*n + 8)*a(n-1)   for n ≥ 4
   Base values:  a(1) = 1,  a(2) = 3,  a(3) = 6
   The function memoises results so calls may come in any order. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, exit */

/*seq_t and idx_t are provided by the build system*/
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 1 ≤ i ≤ cache_size */
    static seq_t  *cache   = NULL;  /* allocated array of seq_t values */
    static size_t  cache_cap = 0;  /* allocated capacity (in elements) */
    static idx_t   cache_size = 0;/* highest index computed so far (≤ cache_cap-1) */

    /* illegal index (offset is 1); nothing is specified for n = 0 */
    if (n == 0)
        return 0;

    /* Ensure the array is large enough for index n (remember index 0 unused). */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;               /* double until sufficient */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);          /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 3;
        } else if (i == 3) {
            cache[3] = 6;
        } else {
            /* coeff = (1/2) * (i^2 - 5*i + 8)  The arithmetic is performed
               in seq_t type, so overflow is taken mod 2^width as required. */
            seq_t i_s = (seq_t)i;
            seq_t coeff = ((i_s * i_s - 5 * i_s + 8) / 2);
            cache[(size_t)i] = cache[(size_t)(i - 1)] * coeff;
        }
    }
    cache_size = n;

    return cache[(size_t)n];
}
