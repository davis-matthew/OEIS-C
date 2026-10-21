/*
 * A096406  a(1) = 4; then alternately add -4 and multiply by -2.
 * https://oeis.org/A096406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=4; a(2n)=a(2n-1)-4; a(2n+1)=a(2n)*(-2)
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache[1..] holds computed values */
    static idx_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_len = 0;     /* largest index computed */

    /* initialise on first call */
    if (cache_len == 0) {
        cache_cap = 2;                      /* enough for indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out of memory – unlikely in tests */
        cache[0] = (seq_t)0;                /* unused */
        cache[1] = (seq_t)4;                /* a(1) */
        cache_len = 1;
    }

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n];

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        if ((k & 1u) == 0u)                /* even indices */
            cache[k] = cache[k - 1] - (seq_t)4;
        else                               /* odd indices */
            cache[k] = cache[k - 1] * (seq_t)(-2);
    }
    cache_len = n;
    return cache[n];
}
