/*
 * A043301  a(n) = 2^n*Sum_{k=0..n} (n+k)!/((n-k)!*k!*4^k).
 * https://oeis.org/A043301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043301
 *  a(0)=1 , a(1)=3
 *  a(n) = (2*n-1)*a(n-1) + 4*a(n-2)  for n>1    (linear recurrence)
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size, in seq_t elements */
    static idx_t   cache_len = 0;  /* number of terms currently cached (>=2 when cache_len>0) */

    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Ensure that the cache can hold up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;   /* start with at least 2 */
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base terms if the cache is fresh */
    if (cache_len == 0) {
        /* The cache is guaranteed to have at least 2 elements at this point */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Compute missing terms with the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];

        seq_t prod = (seq_t)(2 * i - 1) * prev;  /* (2*i-1) * a(i-1) */
        seq_t add  = (seq_t)4 * prev2;           /* 4 * a(i-2) */
        cache[i] = prod + add;
    }

    cache_len = n + 1;
    return cache[n];
}
