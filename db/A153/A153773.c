/*
 * A153773  a(2*n) = 3*a(2*n-1) - 1, a(2*n+1) = 3*a(2*n), with a(1)=1.
 * https://oeis.org/A153773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2n)=3*a(2n-1)-1; a(2n+1)=3*a(2n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* cached values, indices start at 1 */
    static idx_t cache_len = 0;   /* largest index computed so far */
    static size_t cache_cap = 0;  /* allocated size (in elements) */

    if (n == 0)                    /* offset is 1 – ignore n==0 */
        return 0;

    /* ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);        /* allocation failure – abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialization: a(1)=1 */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 1];
        if ((i & 1ULL) == 0)            /* even i : a(2n)=3*a(2n-1)-1 */
            val -= (seq_t)1;
        /* odd i :  a(2n+1)=3*a(2n) – already handled by no subtraction */
        cache[i] = val;
    }
    cache_len = n;

    return cache[n];
}
