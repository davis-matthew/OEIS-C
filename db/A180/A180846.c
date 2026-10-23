/*
 * A180846  a(n) = (81^n - 2^n)/79.
 * https://oeis.org/A180846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (81^n - 2^n)/79 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=83*a(n-1)-162*a(n-2) (for n>=2) (83 and 162 are chosen from
 *            the identity 81 ≡ 2 (mod 79).)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of computed elements    */
    static idx_t cache_cap = 0;   /* allocated capacity              */

    if (n == 0)
        return 0;

    /* Initialise cache on first call or enlarge if needed */
    if (cache_len == 0) {
        /* Ensure capacity for at least two elements */
        cache_cap = (n < 2) ? 2 : n + 1;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure - undefined behaviour */
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    } else if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure - undefined behaviour */
        cache = new_cache;
    }

    /* Return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 83*a(i-1) - 162*a(i-2) */
        seq_t t1 = cache[i-1] * (seq_t)83;
        seq_t t2 = cache[i-2] * (seq_t)162;
        cache[i] = t1 - t2;
    }
    cache_len = n + 1;
    return cache[n];
}
