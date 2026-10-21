/*
 * A072944  a(1)=2, a(n+1) = 2*a(n) - phi(a(n)) where phi is the Euler totient
 *          function A000010.
 * https://oeis.org/A072944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2; a(2)=3; a(3)=4; a(4)=6; a(5)=10; a(n)=4*a(n-3) for all
 *            n>5 (equivalent to a(n+1)=2*a(n)-phi(a(n)))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 95, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to reuse computed values across calls */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_last = 0;

    if (n == (idx_t)0) /* offset is 1; return 0 for out‑of‑range */
        return (seq_t)0;

    /* extend the cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == (idx_t)1)
            cache[i] = (seq_t)2;
        else if (i == (idx_t)2)
            cache[i] = (seq_t)3;
        else if (i == (idx_t)3)
            cache[i] = (seq_t)4;
        else if (i == (idx_t)4)
            cache[i] = (seq_t)6;
        else if (i == (idx_t)5)
            cache[i] = (seq_t)10;
        else
            cache[i] = cache[i - (idx_t)3] * (seq_t)4; /* a(n)=4*a(n-3) */
    }

    cache_last = n;
    return cache[n];
}
